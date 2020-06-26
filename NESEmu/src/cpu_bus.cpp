#include "cpu_bus.h"
#include "cpu6502.h"
#include "cpu_ram.h"
#include "ppu2C02.h"

CPUBus::CPUBus(CPU6502 *cpu, CPURam* cpu_ram, PPU2C02* ppu) : cpu(*cpu), cpu_ram(*cpu_ram), ppu(*ppu) {
	cpu->connect_bus(this);
	ppu->connect_bus(this);
}

u8 CPUBus::read(u16 addr) {
	return cpu_ram.read(addr);
}

void CPUBus::write(u16 addr, u8 value) {
	cpu_ram.write(addr, value);
}
