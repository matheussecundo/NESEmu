#include "cpu6502.h"

#include <iostream>

CPU6502::REG::REG() : PC(0xC000), SP(0xFF), A(0), X(0), Y(0), P(0) {}

void CPU6502::operate() {
	u16 pc = _r.PC;
	u8 op = read(_r.PC++);

	auto info = INSTRUCTION_INFO_ARRAY[op];
	u8 status[] = "nvubdizc";
	for (int i = 0; i < 8; i++) {
		status[7-i] = status[7-i] & (_r.P[i] ? 0b11011111 : 0b11111111);
	}
	printf("$%04X (%s) %s ", pc, info.addrmode, info.operation);
	for (int i = 1; i < opsize[op]; i++) {
		printf("%02X ", read(pc + i));
	}
	printf("A:%02X X:%02X Y:%02X SP:%02X P:%02X:%s\n",  _r.A, _r.X, _r.Y, _r.SP, _r.P.to_ulong(), status);
	printf("stack: ");
	for (int i = _r.SP + 1; i <= 0xFF; i++) {
		printf("%X, ", *bus->cpu_ram[i + 0x100]);
	}
	printf("\n");
	
    if (_reset) {
        reset();
		_reset = 0;
	} else if (_nmi) {
		nmi();
		_nmi = 0;
	} else if (_irq && !get_flag(SR::I)) {
		irq();
		_irq = 0;
	} else {
		(this->*ins[op])();
    }
	std::cin.get();
}

void CPU6502::reset() {
	_r.A = 0;
	_r.X = 0;
	_r.Y = 0;
	_r.SP = 0xFF;
	_r.P = 0;

	u16 addr = 0xFFFC;
	u8 LL = read(addr);
	u8 HH = read(addr + 1);
	_r.PC = (HH << 8) | LL;
}

void CPU6502::connect_bus(CPUBus* bus) {
	this->bus = bus;
}

u8 CPU6502::read(u16 addr) {
	return bus->read(addr);
}

void CPU6502::write(u16 addr, u8 value) {
	bus->write(addr, value);
}

u8 CPU6502::pull() {
	return bus->read(++_r.SP + 0x0100);
}

void CPU6502::push(u8 value) {
	bus->write(_r.SP-- + 0x0100, value);
}

void CPU6502::irq() {
	push(_r.PC >> 8);
	push(_r.PC & 0xFF);
	set_flag(SR::B, 0);
	set_flag(SR::I, 1);
	push(static_cast<u8>(_r.P.to_ulong()));

	u16 addr = 0xFFFE;
	u8 LL = read(addr);
	u8 HH = read(addr + 1);
	_r.PC = (HH << 8) | LL;
}

void CPU6502::nmi() {
	push(_r.PC >> 8);
    push(_r.PC & 0xFF);
	set_flag(SR::B, 0);
	set_flag(SR::I, 1);
	push(static_cast<u8>(_r.P.to_ulong()));

	u16 addr = 0xFFFA;
	u8 LL = read(addr);
	u8 HH = read(addr + 1);
	_r.PC = (HH << 8) | LL;
}

bool CPU6502::get_flag(SR sr) {
	return _r.P[sr];
}

void CPU6502::set_flag(SR sr, bool value) {
	_r.P[sr] = value;
}

// Adressing Modes
#define ADDRESSMODE(F) CPU6502::FETCH CPU6502::F()
ADDRESSMODE (ACC) {
	return { 0, &_r.A };
}
ADDRESSMODE (ABS) {
	u8 LL = read(_r.PC++);
	u8 HH = read(_r.PC++);
	u16 addr = (HH << 8) | LL;
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (ABX) {
	u8 LL = read(_r.PC++);
	u8 HH = read(_r.PC++);
	u16 addr = (HH << 8) | LL;
	addr += _r.X;
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (ABY) {
	u8 LL = read(_r.PC++);
	u8 HH = read(_r.PC++);
	u16 addr = (HH << 8) | LL;
	addr += _r.Y;
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (IMM) {
	u16 addr = _r.PC++;
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (IMP) {
	return { 0, nullptr };
}
ADDRESSMODE (IND) {
    u8 LL = read(_r.PC++);
    u8 HH = read(_r.PC++);
    u16 addr = (HH << 8) | LL;
    LL = read(addr);
    HH = read(addr + 1);
    addr = (HH << 8) | LL;
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (IZX) {
    u8 LL = read(_r.PC++);
    u8 addr8b = LL + _r.X;
    LL = read(addr8b);
    u8 HH = read(addr8b + 1);
	u16 addr = (HH << 8) | LL;
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (IZY) {
    u8 addr8b = read(_r.PC++);
    u8 LL = read(addr8b);
    u8 HH = read(addr8b + 1);
    u16 addr = (HH << 8) | LL;
	addr += _r.Y;
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (REL) {
	u16 addr = _r.PC++;
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (ZP0) {
    u8 addr = read(_r.PC++);
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (ZPY) {
    u8 addr = read(_r.PC++) + _r.Y;
	return { addr, bus->cpu_ram[addr] };
}
ADDRESSMODE (ZPX) {
    u8 addr = read(_r.PC++) + _r.X;
	return { addr, bus->cpu_ram[addr] };
}
#undef ADDRESSMODE

#define FETCH auto fetch = (this->*M)(); u16 addr_bus = fetch.addr_bus; u8* m = fetch.data;
// Instructions
#define INSTRUCTION(F) template<CPU6502::Addrmode M> void CPU6502::F()
INSTRUCTION (ADC) {
	FETCH
	u8 c = get_flag(SR::C);
	u16 l = _r.A + *m + c;
	set_flag(SR::V, (((_r.A ^ *m) & 0x80) ^ 0x80) & ((_r.A ^ l) & 0x80));
	set_flag(SR::C, l >> 8);
	_r.A = static_cast<u8>(l);
	set_flag(SR::N, _r.A >> 7);
	set_flag(SR::Z, !_r.A);
}
INSTRUCTION (AND) {
	FETCH
	_r.A = _r.A & *m;
	set_flag(SR::N, _r.A >> 7);
	set_flag(SR::Z, !_r.A);
}
INSTRUCTION (ASL) {
	FETCH
	set_flag(SR::C, *m >> 7);
	*m = *m << 1;
	set_flag(SR::N, *m >> 7);
	set_flag(SR::Z, !*m);
}
INSTRUCTION (BCC) {
	FETCH
	s8 oper = static_cast<s8>(*m);
	if (!get_flag(SR::C)) _r.PC += oper;
}
INSTRUCTION (BCS) {
	FETCH
	s8 oper = static_cast<s8>(*m);
	if (get_flag(SR::C)) _r.PC += oper;
}
INSTRUCTION (BEQ) {
	FETCH
	s8 oper = static_cast<s8>(*m);
	if (get_flag(SR::Z)) _r.PC += oper;
}
INSTRUCTION (BIT) {
	FETCH
	set_flag(SR::Z, !(_r.A & *m));
	set_flag(SR::N, *m >> 7);
	set_flag(SR::V, *m & 0x40);
}
INSTRUCTION (BMI) {
	FETCH
	s8 oper = static_cast<s8>(*m);
	if (get_flag(SR::N)) _r.PC += oper;
}
INSTRUCTION (BNE) {
	FETCH
	s8 oper = static_cast<s8>(*m);
	if (!get_flag(SR::Z)) _r.PC += oper;
}
INSTRUCTION (BPL) {
	FETCH
	s8 oper = static_cast<s8>(*m);
	if (!get_flag(SR::N)) _r.PC += oper;
}
INSTRUCTION (BRK) {
	FETCH
	set_flag(SR::I, 1);
	u16 addr = _r.PC + 1;
	push(addr >> 8);
	push(addr & 0xFF);
	push(static_cast<u8>(_r.P.to_ulong()));
	_r.PC = 0xFFF0;
}
INSTRUCTION (BVC) {
	FETCH
	s8 oper = static_cast<s8>(*m);
	if (!get_flag(SR::V)) _r.PC += oper;
}
INSTRUCTION (BVS) {
	FETCH
	s8 oper = static_cast<s8>(*m);
	if (get_flag(SR::V)) _r.PC += oper;
}
INSTRUCTION (CLC) {
	FETCH
	set_flag(SR::C, 0);
}
INSTRUCTION (CLD) {
	FETCH
	set_flag(SR::D, 0);
}
INSTRUCTION (CLI) {
	FETCH
	set_flag(SR::I, 0);
}
INSTRUCTION (CLV) {
	FETCH
	set_flag(SR::V, 0);
}
INSTRUCTION (CMP) {
	FETCH
	u16 l = _r.A - *m;
	set_flag(SR::C, ((l >> 8) & 0x01) ^ 0x01);
	set_flag(SR::N, (l & 0xff) >> 7);
	set_flag(SR::Z, !(l & 0xff));
}
INSTRUCTION (CPX) {
	FETCH
	u16 l = _r.X - *m;
	set_flag(SR::C, ((l >> 8) & 0x01) ^ 0x01);
	set_flag(SR::N, (l & 0xff) >> 7);
	set_flag(SR::Z, !(l & 0xff));
}
INSTRUCTION (CPY) {
	FETCH
	u16 l = _r.Y - *m;
	set_flag(SR::C, ((l >> 8) & 0x01) ^ 0x01);
	set_flag(SR::N, (l & 0xff) >> 7);
	set_flag(SR::Z, !(l & 0xff));
}
INSTRUCTION (DEC) {
	FETCH
	(*m)--;
	set_flag(SR::N, *m >> 7);
	set_flag(SR::Z, !*m);
}
INSTRUCTION (DEX) {
	FETCH
	_r.X--;
	set_flag(SR::N, _r.X >> 7);
	set_flag(SR::Z, !_r.X);
}
INSTRUCTION (DEY) {
	FETCH
	_r.Y--;
	set_flag(SR::N, _r.Y >> 7);
	set_flag(SR::Z, !_r.Y);
}
INSTRUCTION (EOR) {
	FETCH
	_r.A = _r.A ^ *m;
	set_flag(SR::N, _r.A >> 7);
	set_flag(SR::Z, !_r.A);
}
INSTRUCTION (INC) {
	FETCH
	(*m)++;
	set_flag(SR::N, *m >> 7);
	set_flag(SR::Z, !*m);
}
INSTRUCTION (INX) {
	FETCH
	_r.X++;
	set_flag(SR::N, _r.X >> 7);
	set_flag(SR::Z, !_r.X);
}
INSTRUCTION (INY) {
	FETCH
	_r.Y++;
	set_flag(SR::N, _r.Y >> 7);
	set_flag(SR::Z, !_r.Y);
}
INSTRUCTION (JMP) {
	FETCH
	_r.PC = addr_bus;
}
INSTRUCTION (JSR) {
	FETCH
	push((_r.PC - 1) >> 8);
	push((_r.PC - 1) & 0xFF);
	_r.PC = addr_bus;
}
INSTRUCTION (LDA) {
	FETCH
	_r.A = *m;
	set_flag(SR::N, _r.A >> 7);
	set_flag(SR::Z, !_r.A);
}
INSTRUCTION (LDX) {
	FETCH
	_r.X = *m;
	set_flag(SR::N, _r.X >> 7);
	set_flag(SR::Z, !_r.X);
}
INSTRUCTION (LDY) {
	FETCH
	_r.Y = *m;
	set_flag(SR::N, _r.Y >> 7);
	set_flag(SR::Z, !_r.Y);
}
INSTRUCTION (LSR) {
	FETCH
	set_flag(SR::C, *m & 0x01);
	*m = *m >> 1;
	set_flag(SR::Z, !*m);
}
INSTRUCTION (NOP) {

}
INSTRUCTION (ORA) {
	FETCH
	_r.A = _r.A | *m ;
	set_flag(SR::N, _r.A >> 7);
	set_flag(SR::Z, !_r.A);
}
INSTRUCTION (PHA) {
	FETCH
	push(_r.A);
}
INSTRUCTION (PHP) {
	FETCH
	push(static_cast<u8>(_r.P.to_ulong()));
}
INSTRUCTION (PLA) {
	FETCH
	_r.A = pull();
	set_flag(SR::N, _r.A >> 7);
	set_flag(SR::Z, !_r.A);
}
INSTRUCTION (PLP) {
	FETCH
	_r.P = pull();
}
INSTRUCTION (ROL) {
	FETCH
	u8 c = *m >> 7;
	*m = *m << 1;
	u8 previous_c = get_flag(SR::C);
	*m = *m | previous_c;
	set_flag(SR::C, c);
	set_flag(SR::N, *m >> 7);
	set_flag(SR::Z, !*m);
}
INSTRUCTION (ROR) {
	FETCH
	u8 c = *m & 0x01;
	*m = *m >> 1;
	u8 previous_c = get_flag(SR::C);
	*m = *m | (previous_c << 7);
	set_flag(SR::C, c);
	set_flag(SR::N, *m >> 7);
	set_flag(SR::Z, !*m);
}
INSTRUCTION (RTI) {
	FETCH
	_r.P = pull();
	u8 LL = pull();
	u8 HH = pull();
	_r.PC = (HH << 8) | LL;
}
INSTRUCTION (RTS) {
	FETCH
	u8 LL = pull();
	u8 HH = pull();
	_r.PC = (HH << 8) | LL;
	_r.PC++;
}
INSTRUCTION (SBC) {
	FETCH
	u8 c = get_flag(SR::C);
	u16 l = _r.A - *m - (c ^ 0x01);
	set_flag(SR::V, (_r.A ^ 0x01) & (_r.A ^ *m) & 0x80);
	set_flag(SR::C, ((l >> 8) & 0x01) ^ 0x01);
	_r.A = static_cast<u8>(l);
	set_flag(SR::N, _r.A >> 7);
	set_flag(SR::Z, !_r.A);
}
INSTRUCTION (SEC) {
	FETCH
	set_flag(SR::C, 1);
}
INSTRUCTION (SED) {
	FETCH
	set_flag(SR::D, 1);
}
INSTRUCTION (SEI) {
	FETCH
	set_flag(SR::I, 1);
}
INSTRUCTION (STA) {
	FETCH
	*m = _r.A;
}
INSTRUCTION (STX) {
	FETCH
	*m = _r.X;
}
INSTRUCTION (STY) {
	FETCH
	*m = _r.Y;
}
INSTRUCTION (TAX) {
	FETCH
	_r.X = _r.A;
	set_flag(SR::N, _r.X >> 7);
	set_flag(SR::Z, !_r.X);
}
INSTRUCTION (TAY) {
	FETCH
	_r.Y = _r.A;
	set_flag(SR::N, _r.Y >> 7);
	set_flag(SR::Z, !_r.Y);
}
INSTRUCTION (TSX) {
	FETCH
	_r.X = _r.SP;
	set_flag(SR::N, _r.X >> 7);
	set_flag(SR::Z, !_r.X);
}
INSTRUCTION (TXA) {
	FETCH
	_r.A = _r.X;
	set_flag(SR::N, _r.A >> 7);
	set_flag(SR::Z, !_r.A);
}
INSTRUCTION (TXS) {
	FETCH
	_r.SP = _r.X;
}
INSTRUCTION (TYA) {
	FETCH
	_r.A = _r.Y;
	set_flag(SR::N, _r.A >> 7);
	set_flag(SR::Z, !_r.A);
}

INSTRUCTION (XXX) {

}
#undef INSTRUCTION
#undef FETCH

#define I(OP, ADDRMODE) &CPU6502::OP<&CPU6502::ADDRMODE>
CPU6502::CPU6502() :
    ins(INSTRUCTIONS_ARRAY(I)),
    bus(nullptr)
{}
#undef I