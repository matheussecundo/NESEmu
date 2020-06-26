#include "game_pak.h"

#include <cstdio>
#include <cstring>

GamePak::GamePak(const char* path) :
	VRAM(0x2000)
{
	FILE* ROMfp = fopen(path, "rb");
	readROMfile(ROMfp);
	fclose(ROMfp);
}

bool GamePak::readROMfile(FILE* ROMfp)
{
	//read Header
	if (!(fgetc(ROMfp) == 'N' && fgetc(ROMfp) == 'E' && fgetc(ROMfp) == 'S' && fgetc(ROMfp) == 0x1A))
		return false;

	rom16count = std::fgetc(ROMfp);
	vrom8count = std::fgetc(ROMfp);
	ctrlbyte = std::fgetc(ROMfp);
	mappernum = std::fgetc(ROMfp) | (ctrlbyte >> 4);
	for (u8 i = 0; i < 8; i++)
		std::fgetc(ROMfp);
	if (mappernum >= 0x40) mappernum = mappernum & 0x0F;

	//read Data
	if (rom16count) ROM.resize(static_cast<size_t>(rom16count) * 0x4000);
	if (vrom8count) VRAM.resize(static_cast<size_t>(vrom8count) * 0x2000);
	std::fread(&ROM[0], rom16count, 0x4000, ROMfp);
	std::fread(&VRAM[0], vrom8count, 0x2000, ROMfp);

	std::printf("%u * 16kB ROM, %u * 8kB VROM, mapper %u, ctrlbyte %02X\n", rom16count, vrom8count, mappernum, ctrlbyte);
	return true;
}

void GamePak::copy(CPURam& dest)
{
	switch (mappernum) {
	case  0:
		for (u8 i = 0; i < 2; i++)
			std::memcpy(dest[0x8000 + i * 0x4000], &ROM[static_cast<size_t>(i % rom16count) * 0x4000], 0x4000);
		return;
	case  1:
	case  2:
	case  3:
	case  4:
	case  5:
	case  6:
	case  7:
	case  8:
	case  9:
	case 10:
	case 11:
	case 12:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 32:
	case 33:
	case 34:
	case 64:
	case 65:
	case 66:
	case 67:
	case 68:
	case 69:
	case 71:
	case 78:
	case 91:
	default: return;
	}
}
