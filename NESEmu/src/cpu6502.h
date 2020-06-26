#pragma once

#include <bitset>
#include <array>

#include "core.h"

#include "cpu_ram.h"
#include "cpu_bus.h"

#define INSTRUCTIONS_ARRAY(I)\
{\
    I(BRK, IMP), I(ORA, IZX), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(ORA, ZP0), I(ASL, ZP0), I(XXX, IMP), I(PHP, IMP), I(ORA, IMM), I(ASL, ACC), I(XXX, IMP), I(XXX, IMP), I(ORA, ABS), I(ASL, ABS), I(XXX, IMP),\
    I(BPL, REL), I(ORA, IZY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(ORA, ZPX), I(ASL, ZPX), I(XXX, IMP), I(CLC, IMP), I(ORA, ABY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(ORA, ABX), I(ASL, ABX), I(XXX, IMP),\
    I(JSR, ABS), I(AND, IZX), I(XXX, IMP), I(XXX, IMP), I(BIT, ZP0), I(AND, ZP0), I(ROL, ZP0), I(XXX, IMP), I(PLP, IMP), I(AND, IMM), I(ROL, ACC), I(XXX, IMP), I(BIT, ABS), I(AND, ABS), I(ROL, ABS), I(XXX, IMP),\
    I(BMI, REL), I(AND, IZY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(AND, ZPX), I(ROL, ZPX), I(XXX, IMP), I(SEC, IMP), I(AND, ABY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(AND, ABX), I(ROL, ABX), I(XXX, IMP),\
    I(RTI, IMP), I(EOR, IZX), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(EOR, ZP0), I(LSR, ZP0), I(XXX, IMP), I(PHA, IMP), I(EOR, IMM), I(LSR, ACC), I(XXX, IMP), I(JMP, ABS), I(EOR, ABS), I(LSR, ABS), I(XXX, IMP),\
    I(BVC, REL), I(EOR, IZY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(EOR, ZPX), I(LSR, ZPX), I(XXX, IMP), I(CLI, IMP), I(EOR, ABY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(EOR, ABX), I(LSR, ABX), I(XXX, IMP),\
    I(RTS, IMP), I(ADC, IZX), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(ADC, ZP0), I(ROR, ZP0), I(XXX, IMP), I(PLA, IMP), I(ADC, IMM), I(ROR, ACC), I(XXX, IMP), I(JMP, IND), I(ADC, ABS), I(ROR, ABS), I(XXX, IMP),\
    I(BVS, REL), I(ADC, IZY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(ADC, ZPX), I(ROR, ZPX), I(XXX, IMP), I(SEI, IMP), I(ADC, ABY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(ADC, ABX), I(ROR, ABX), I(XXX, IMP),\
    I(XXX, IMP), I(STA, IZX), I(XXX, IMP), I(XXX, IMP), I(STY, ZP0), I(STA, ZP0), I(STX, ZP0), I(XXX, IMP), I(DEY, IMP), I(XXX, IMP), I(TXA, IMP), I(XXX, IMP), I(STY, ABS), I(STA, ABS), I(STX, ABS), I(XXX, IMP),\
    I(BCC, REL), I(STA, IZY), I(XXX, IMP), I(XXX, IMP), I(STY, ZPX), I(STA, ZPX), I(STX, ZPY), I(XXX, IMP), I(TYA, IMP), I(STA, ABY), I(TXS, IMP), I(XXX, IMP), I(XXX, IMP), I(STA, ABX), I(XXX, ABS), I(XXX, IMP),\
    I(LDY, IMM), I(LDA, IZX), I(LDX, IMM), I(XXX, IMP), I(LDY, ZP0), I(LDA, ZP0), I(LDX, ZP0), I(XXX, IMP), I(TAY, IMP), I(LDA, IMM), I(TAX, IMP), I(XXX, IMP), I(LDY, ABS), I(LDA, ABS), I(LDX, ABS), I(XXX, IMP),\
    I(BCS, REL), I(LDA, IZY), I(XXX, IMP), I(XXX, IMP), I(LDY, ZPX), I(LDA, ZPX), I(LDX, ZPY), I(XXX, IMP), I(CLV, IMP), I(LDA, ABY), I(TSX, IMP), I(XXX, IMP), I(LDY, ABX), I(LDA, ABX), I(LDX, ABY), I(XXX, IMP),\
    I(CPY, IMM), I(CMP, IZX), I(XXX, IMP), I(XXX, IMP), I(CPY, ZP0), I(CMP, ZP0), I(DEC, ZP0), I(XXX, IMP), I(INY, IMP), I(CMP, IMM), I(DEX, IMP), I(XXX, IMP), I(CPY, ABS), I(CMP, ABS), I(DEC, ABS), I(XXX, IMP),\
    I(BNE, REL), I(CMP, IZY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(CMP, ZPX), I(DEC, ZPX), I(XXX, IMP), I(CLD, IMP), I(CMP, ABY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(CMP, ABX), I(DEC, ABX), I(XXX, IMP),\
    I(CPX, IMM), I(SBC, IZX), I(XXX, IMP), I(XXX, IMP), I(CPX, ZP0), I(SBC, ZP0), I(INC, ZP0), I(XXX, IMP), I(INX, IMP), I(SBC, IMM), I(NOP, IMP), I(XXX, IMP), I(CPX, ABS), I(SBC, ABS), I(INC, ABS), I(XXX, IMP),\
    I(BEQ, REL), I(SBC, IZY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(SBC, ZPX), I(INC, ZPX), I(XXX, IMP), I(SED, IMP), I(SBC, ABY), I(XXX, IMP), I(XXX, IMP), I(XXX, IMP), I(SBC, ABX), I(INC, ABX), I(XXX, IMP)\
}

struct INSTRUCTION_INFO {
    const char* operation;
    const char* addrmode;
};

#define I(OP, ADDRMODE) INSTRUCTION_INFO {#OP, #ADDRMODE}
constexpr std::array<INSTRUCTION_INFO, 256> INSTRUCTION_INFO_ARRAY(INSTRUCTIONS_ARRAY(I));
#undef I

constexpr u8 opcycle[256] =
{
	/*0x00*/    7,6,2,8,3,3,5,5,3,2,2,2,4,4,6,6,
	/*0x10*/    2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0x20*/    6,6,2,8,3,3,5,5,4,2,2,2,4,4,6,6,
	/*0x30*/    2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0x40*/    6,6,2,8,3,3,5,5,3,2,2,2,3,4,6,6,
	/*0x50*/    2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0x60*/    6,6,2,8,3,3,5,5,4,2,2,2,5,4,6,6,
	/*0x70*/    2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0x80*/    2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4,
	/*0x90*/    2,6,2,6,4,4,4,4,2,5,2,5,5,5,5,5,
	/*0xA0*/    2,6,2,6,3,3,3,3,2,2,2,2,4,4,4,4,
	/*0xB0*/    2,5,2,5,4,4,4,4,2,4,2,4,4,4,4,4,
	/*0xC0*/    2,6,2,8,3,3,5,5,2,2,2,2,4,4,6,6,
	/*0xD0*/    2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7,
	/*0xE0*/    2,6,3,8,3,3,5,5,2,2,2,2,4,4,6,6,
	/*0xF0*/    2,5,2,8,4,4,6,6,2,4,2,7,4,4,7,7
};

constexpr u8 opsize[256] =
{
	/*0x00*/    1,2,0,0,0,2,2,0,1,2,1,0,0,3,3,0,
	/*0x10*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
	/*0x20*/	3,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,
	/*0x30*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
	/*0x40*/	1,2,0,0,0,2,2,0,1,2,1,0,3,3,3,0,
	/*0x50*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
	/*0x60*/	1,2,0,0,0,2,2,0,1,2,1,0,3,3,3,0,
	/*0x70*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
	/*0x80*/	0,2,0,0,2,2,2,0,1,0,1,0,3,3,3,0,
	/*0x90*/	2,2,0,0,2,2,2,0,1,3,1,0,0,3,0,0,
	/*0xA0*/	2,2,2,0,2,2,2,0,1,2,1,0,3,3,3,0,
	/*0xB0*/	2,2,0,0,2,2,2,0,1,3,1,0,3,3,3,0,
	/*0xC0*/	2,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,
	/*0xD0*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0,
	/*0xE0*/	2,2,0,0,2,2,2,0,1,2,1,0,3,3,3,0,
	/*0xF0*/	2,2,0,0,0,2,2,0,1,3,0,0,0,3,3,0
};

class CPU6502
{
public:
	enum SR { C = 0, Z, I, D, B, U, V, N };
    struct REG {
        REG();

        u16 PC;
        u8 SP;
        u8 A;
        u8 X;
        u8 Y;
        std::bitset<8> P;
    };
    struct FETCH {
        u16 addr_bus;
        u8* data;
    };
    typedef void(CPU6502::* Operation)(void);
    typedef FETCH(CPU6502::* Addrmode)(void);

	CPU6502();
	void operate();
	void reset();
	void connect_bus(CPUBus*);
private:
	u8 read(u16);
	void write(u16, u8);
	u8 pull();
	void push(u8);

	void irq();
	void nmi();

	bool get_flag(SR);
	void set_flag(SR, bool);

    const std::array<Operation, 256> ins;
	CPUBus* bus;

	bool _reset = 1, _irq = 0, _nmi = 0;
    REG _r;

	/*
		Address Modes:

		A		....	Accumulator	 	OPC		A	 			operand is AC (implied single byte instruction)
		abs		....	absolute	 	OPC		$LLHH	 		operand is address $HHLL *
		abs,X	....	absolute, X-indexed	 	OPC $LLHH,X	 	operand is address; effective address is address incremented by X with carry **
		abs,Y	....	absolute, Y-indexed	 	OPC $LLHH,Y	 	operand is address; effective address is address incremented by Y with carry **
		#		....	immediate	 	OPC		#$BB	 		operand is byte BB
		impl	....	implied	 		OPC	 	operand			implied
		ind		....	indirect	 	OPC		($LLHH)	 		operand is address; effective address is contents of word at address: C.w($HHLL)
		X,ind	....	X-indexed, indirect	 	OPC ($LL,X)	 	operand is zeropage address; effective address is word in (LL + X, LL + X + 1), inc. without carry: C.w($00LL + X)
		ind,Y	....	indirect, Y-indexed	 	OPC ($LL),Y	 	operand is zeropage address; effective address is word in (LL, LL + 1) incremented by Y with carry: C.w($00LL) + Y
		rel		....	relative	 	OPC		$BB	 			branch target is PC + signed offset BB ***
		zpg		....	zeropage	 	OPC		$LL	 			operand is zeropage address (hi-byte is zero, address = $00LL)
		zpg,X	....	zeropage, X-indexed	 	OPC $LL,X	 	operand is zeropage address; effective address is address incremented by X without carry **
		zpg,Y	....	zeropage, Y-indexed	 	OPC $LL,Y	 	operand is zeropage address; effective address is address incremented by Y without carry **
	*/
    #define ADDRESSMODE(F) FETCH F()
    ADDRESSMODE (ACC);
    ADDRESSMODE (ABS);
    ADDRESSMODE (ABX);
    ADDRESSMODE (ABY);
    ADDRESSMODE (IMM);
    ADDRESSMODE (IMP);
    ADDRESSMODE (IND);
    ADDRESSMODE (IZX);
    ADDRESSMODE (IZY);
    ADDRESSMODE (REL);
    ADDRESSMODE (ZP0);
    ADDRESSMODE (ZPY);
    ADDRESSMODE (ZPX);
    #undef ADDRESSMODE

	// Instructions
    #define INSTRUCTION(F) template<Addrmode> void F()
    INSTRUCTION (ADC);
    INSTRUCTION (AND);
    INSTRUCTION (ASL);
    INSTRUCTION (BCC);
    INSTRUCTION (BCS);
    INSTRUCTION (BEQ);
    INSTRUCTION (BIT);
    INSTRUCTION (BMI);
    INSTRUCTION (BNE);
    INSTRUCTION (BPL);
    INSTRUCTION (BRK);
    INSTRUCTION (BVC);
    INSTRUCTION (BVS);
    INSTRUCTION (CLC);
    INSTRUCTION (CLD);
    INSTRUCTION (CLI);
    INSTRUCTION (CLV);
    INSTRUCTION (CMP);
    INSTRUCTION (CPX);
    INSTRUCTION (CPY);
    INSTRUCTION (DEC);
    INSTRUCTION (DEX);
    INSTRUCTION (DEY);
    INSTRUCTION (EOR);
    INSTRUCTION (INC);
    INSTRUCTION (INX);
    INSTRUCTION (INY);
    INSTRUCTION (JMP);
    INSTRUCTION (JSR);
    INSTRUCTION (LDA);
    INSTRUCTION (LDX);
    INSTRUCTION (LDY);
    INSTRUCTION (LSR);
    INSTRUCTION (NOP);
    INSTRUCTION (ORA);
    INSTRUCTION (PHA);
    INSTRUCTION (PHP);
    INSTRUCTION (PLA);
    INSTRUCTION (PLP);
    INSTRUCTION (ROL);
    INSTRUCTION (ROR);
    INSTRUCTION (RTI);
    INSTRUCTION (RTS);
    INSTRUCTION (SBC);
    INSTRUCTION (SEC);
    INSTRUCTION (SED);
    INSTRUCTION (SEI);
    INSTRUCTION (STA);
    INSTRUCTION (STX);
    INSTRUCTION (STY);
    INSTRUCTION (TAX);
    INSTRUCTION (TAY);
    INSTRUCTION (TSX);
    INSTRUCTION (TXA);
    INSTRUCTION (TXS);
    INSTRUCTION (TYA);

    INSTRUCTION (XXX);
    #undef INSTRUCTION
};
