#pragma once
#include <cstdint>
#include <stdio.h>
#include <string.h>

class memory
{
private:
	uint8_t memarray[0x1000];

public:
	memory();
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t addr);
	void wipe_memory();
};

