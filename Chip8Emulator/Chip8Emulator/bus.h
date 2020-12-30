#pragma once
#include "chip8cpu.h"
#include "memory.h"
#include <vector>
#include <Windows.h>

class Bus
{
public:
	Bus();
	~Bus();
	
	chip8cpu cpu;
	memory mem;
	uint64_t display[32]{ 0 };
	void get_keystate();

};

