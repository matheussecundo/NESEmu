#pragma once

#include <array>

#include "core.h"

class CPURam
{
public:
	CPURam();
	u8 read(u16);
	void write(u16, u8);
	u8* operator[](u16);
private:
	// ZEROPAGE + STACK + RAM
	std::array<u8, 0x0800 - 0x0000> ram;
	// I/O Registers 0
	std::array<u8, 0x2008 - 0x2000> io_registers_0;
	// I/O Registers 1
	std::array<u8, 0x4020 - 0x4000> io_registers_1;
	// Expansion ROM
	std::array<u8, 0x6000 - 0x4020> expansion_rom;
	// SRAM
	std::array<u8, 0x8000 - 0x6000> sram;
	// PRG LOWER BANK
	std::array<u8, 0xC000 - 0x8000> prg_l;
	// PRG UPPER BANK
	std::array<u8, 0x10000 - 0xC000> prg_u;

	// Mapped ram
	std::array<u8*, 0x10000> mem;
};

