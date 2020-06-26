#pragma once

#include <array>

#include "core.h"

class PPURam
{
public:
	PPURam();
	u8 read(u16);
	void write(u16, u8);
	u8* operator[](u16);
private:
	std::array<u8, 0x2000 - 0x0000> pattern_tables;
	std::array<u8, 0x3000 - 0x2000> name_tables;
	std::array<u8, 0x3F20 - 0x3F00> palettes;

	// Mapped ram
	std::array<u8*, 0x10000> mem;
};

