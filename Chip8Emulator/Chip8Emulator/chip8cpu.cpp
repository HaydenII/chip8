#include "chip8cpu.h"
#include "bus.h"

chip8cpu::chip8cpu()
{
	uint16_t mem_pointer;

	using a = chip8cpu;

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

void chip8cpu::clock()
{
	opcode = read(pc);
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

inline void chip8cpu::DecrementStackPointer()
{
	if (sp != 0x0) {
		sp--;
	}
}

void chip8cpu::SYS()
{
	pc++;
	hi = read(pc);
	pc++;
	lo = read(pc);
	pc++;

	pc = (hi << 8) | lo;

}

void chip8cpu::RET()
{
	pc = stack[sp];
	DecrementStackPointer;
}

void chip8cpu::JP()
{
	pc++;
	hi = read(pc);
	pc++;
	lo = read(pc);
	pc++;

	pc = (hi << 8) | lo;
}

void chip8cpu::CALL() 
{
	sp++;
	stack[sp] = pc;

	pc++;
	hi = read(pc);
	pc++;
	lo = read(pc);
	pc++;

	pc = (hi << 8) | lo;
}

void chip8cpu::SE()
{

}
