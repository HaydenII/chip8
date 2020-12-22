#include "memory.h"

memory::memory()
{
	for (auto &b : memarray) {
		b = 0x00;
	}
}

void memory::write(uint16_t addr, uint8_t data)
{
	if (addr <= 0x1000) {
		memarray[addr] = data;
	}
}

uint8_t memory::read(uint16_t addr)
{
	if (addr <= 0x1000) {
		return memarray[addr];
	}
	return 0x00;
}
