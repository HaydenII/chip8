#pragma once
#include <cstdint>

#include "chip8cpu.h"

class memory
{
private:
	uint8_t memarray[0x1000];
	chip8cpu* cpu;

public:
	memory();
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);
};

