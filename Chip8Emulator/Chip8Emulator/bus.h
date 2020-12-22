#pragma once
#include "chip8cpu.h"
#include "memory.h"

class Bus
{
public:
	Bus();
	~Bus();

	chip8cpu cpu;
	memory mem;

};

