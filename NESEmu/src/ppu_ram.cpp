#include "ppu_ram.h"

PPURam::PPURam() :
	pattern_tables{ 0 },
	name_tables{ 0 },
	palettes{ 0 },
	mem{ nullptr }
{
	u32 j = 0;
	while (j < 0x10000) {
		for (u32 i = 0; i < 0x2000 - 0x0000; i++) {
			mem[j++] = &pattern_tables[i];
		}
		for (u32 i = 0; i < 0x3000 - 0x2000; i++) {
			mem[j++] = &name_tables[i];
		}

		while (j < 0x3F00) {
			for (u32 i = 0; i < 0x2F00 - 0x2000; i++) {
				mem[j++] = &name_tables[i];
			}
		}

		while (j < 0x4000) {
			for (u32 i = 0; i < 0x3F20 - 0x3F00; i++) {
				mem[j++] = &palettes[i];
			}
		}
	}
}

u8 PPURam::read(u16 addr) {
	return *mem[addr];
}

void PPURam::write(u16 addr, u8 value) {
	*mem[addr] = value;
}

u8* PPURam::operator[](u16 addr) {
	return mem[addr];
}