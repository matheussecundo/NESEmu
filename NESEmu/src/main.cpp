#include <iostream>

#include "cpu_bus.h"
#include "cpu6502.h"
#include "cpu_ram.h"
#include "ppu2C02.h"
#include "game_pak.h"

int main(int argc, char** argv) {
    if (argc <= 1) {
        return -1;
    }
    GamePak gamepak{argv[1]};

    CPU6502 cpu;
    CPURam *cpu_ram = new CPURam;
    PPU2C02 *ppu = new PPU2C02;
    CPUBus bus(&cpu, cpu_ram, ppu);

    gamepak.copy(*cpu_ram);

    for (;;) {
        cpu.operate();
    }
}
