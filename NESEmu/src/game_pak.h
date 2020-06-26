#pragma once

#include <array>
#include <vector>

#include "core.h"

#include "cpu_ram.h"

class GamePak
{
public:
	GamePak(const char*);
	void copy(CPURam&);
private:
	const char* path;
	u8 rom16count;
	u8 vrom8count;
	u8 ctrlbyte;
	u8 mappernum;
	std::vector<u8> ROM;
	std::vector<u8> VRAM;

	bool readROMfile(FILE*);
};

