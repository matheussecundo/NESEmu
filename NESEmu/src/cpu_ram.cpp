#include "cpu_ram.h"

CPURam::CPURam() :
	ram{ 0 },
	io_registers_0{ 0 },
	io_registers_1{ 0 },
	expansion_rom{ 0 },
	sram{ 0 },
	prg_l{ 0 },
	prg_u{ 0 },
	mem{ nullptr }
{
	u32 j = 0;
	while (j < 0x2000) {
		for (u32 i = 0; i < 0x0800 - 0x0000; i++) {
			mem[j++] = &ram[i];
		}
	}
	while (j < 0x4000) {
		for (u32 i = 0; i < 0x2008 - 0x2000; i++) {
			mem[j++] = &io_registers_0[i];
		}
	}
	for (u32 i = 0; i < 0x4020 - 0x4000; i++) {
		mem[j++] = &io_registers_1[i];
	}
	for (u32 i = 0; i < 0x6000 - 0x4020; i++) {
		mem[j++] = &expansion_rom[i];
	}
	for (u32 i = 0; i < 0x8000 - 0x6000; i++) {
		mem[j++] = &sram[i];
	}
	for (u32 i = 0; i < 0xC000 - 0x8000; i++) {
		mem[j++] = &prg_l[i];
	}
	for (u32 i = 0; i < 0x10000 - 0xC000; i++) {
		mem[j++] = &prg_u[i];
	}
}

u8 CPURam::read(u16 addr) {
	return *mem[addr];
}

void CPURam::write(u16 addr, u8 value) {
	*mem[addr] = value;
}

u8* CPURam::operator[](u16 addr) {
	return mem[addr];
}
