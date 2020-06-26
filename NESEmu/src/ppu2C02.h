#pragma once

#include "cpu_bus.h"

#include "core.h"
#include "ppu_ram.h"

class PPU2C02
{
public:
	PPU2C02();
	void operate();
	void reset();
	void connect_bus(CPUBus*);
private:
	PPURam ram;
	CPUBus* bus;
};

