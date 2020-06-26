#pragma once

#include "core.h"

class CPU6502;
class CPURam;
class PPU2C02;

class CPUBus
{
public:
    CPUBus(CPU6502*, CPURam*, PPU2C02*);
    u8 read(u16);
    void write(u16, u8);

    CPU6502& cpu;
    CPURam& cpu_ram;
    PPU2C02& ppu;
};
