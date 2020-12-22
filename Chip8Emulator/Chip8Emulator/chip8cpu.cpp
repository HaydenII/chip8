#include "chip8cpu.h"
#include "bus.h"

chip8cpu::chip8cpu()
{
	uint16_t mem_pointer;

	// For later, loads sprites into memory
	//uint8_t sprites[5][16] = {
	//	{0xF0, 0x90, 0x90, 0x90, 0xF0},
	//	{0x20, 0x60, 0x20, 0x20, 0x70},
	//	{0xF0, 0x10, 0xF0, 0x80, 0xF0},
	//	{0xF0, 0x10, 0xF0, 0x10, 0xF0},
	//	{0x90, 0x90, 0xF0, 0x10, 0x10}
	//};

	//for (uint16_t MemPtr = 0x000; MemPtr < 0x200; MemPtr++) {
	//	write(MemPtr, sprites[0]);
	//}
}

chip8cpu::~chip8cpu()
{
}

uint8_t chip8cpu::read(uint16_t addr)
{
	return bus->mem.read(addr);
}

void chip8cpu::write(uint16_t addr, uint8_t data)
{
	bus->mem.write(addr, data);
}

void chip8cpu::ConnectBus(Bus* busptr)
{
	bus = busptr;
}
