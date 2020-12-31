#pragma once
#include "renderer.h"
#include <Windows.h>

#include "cpu.h"
#include "memory.h"

class Bus
{
public:
	Bus();
	
	chip8cpu cpu;
	memory mem;
	uint64_t display[32]{ 0 };
	void get_keystate();
};
