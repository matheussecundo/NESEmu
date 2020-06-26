#include "ppu2C02.h"

PPU2C02::PPU2C02() : bus(nullptr) {

}

void PPU2C02::operate() {

}

void PPU2C02::reset() {

}

void PPU2C02::connect_bus(CPUBus* bus) {
	this->bus = bus;
}