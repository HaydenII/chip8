#include "chip8cpu.h"
#include "bus.h"

uint16_t chip8cpu::ReadNext2Bytes()
{
	pc++;
	hi = read(pc);
	pc++;
	lo = read(pc);
	pc++;

	return (hi << 8) | lo;
}

chip8cpu::chip8cpu()
{
	uint16_t mem_pointer;

	using a = chip8cpu;
}

chip8cpu::~chip8cpu()
{
}

void chip8cpu::clock()
{
	instruction16Bit = ReadNext2Bytes();
	if (instruction16Bit == 0x00E0) { SYS(); }
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
	pc = (instruction16Bit & 0x0FFF);
}

// UNINPLEMENTED
void chip8cpu::CLS()
{
}

void chip8cpu::RET()
{
	pc = stack[sp];
	DecrementStackPointer();
}

void chip8cpu::JP()
{
	pc = (instruction16Bit & 0x0FFF);
}

void chip8cpu::CALL() 
{
	sp++;
	stack[sp] = pc;
	pc = (instruction16Bit & 0x0FFF);
}

void chip8cpu::SE()
{
	uint8_t val = (instruction16Bit & 0x00FF);
	uint8_t reg = regs[((instruction16Bit >> 8) & 0x000F)];
	if (val == reg) {
		pc += 2;
	}
}

void chip8cpu::SNE()
{
	uint8_t val = (instruction16Bit & 0x00FF);
	uint8_t reg = regs[((instruction16Bit >> 8) & 0x000F)];
	if (val != reg) {
		pc += 2;
	}
}

void chip8cpu::SE_2()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	uint8_t regY = regs[((instruction16Bit >> 4) & 0x000F)];
	if (regX == regY) {
		pc += 2;
	}
}

void chip8cpu::LD()
{
	uint8_t val = (instruction16Bit & 0x00FF);
	regs[((instruction16Bit >> 8) & 0x000F)] = val;
}

void chip8cpu::ADD()
{
	uint8_t val = (instruction16Bit & 0x00FF);
	regs[((instruction16Bit >> 8) & 0x000F)] += val;
}

void chip8cpu::LD_2()
{
	uint8_t regY = regs[((instruction16Bit >> 4) & 0x000F)];
	regs[((instruction16Bit >> 8) & 0x000F)] = regY;
}

void chip8cpu::OR()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	uint8_t regY = regs[((instruction16Bit >> 4) & 0x000F)];
	regs[((instruction16Bit >> 8) & 0x000F)] = (regX | regY);
}

void chip8cpu::AND()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	uint8_t regY = regs[((instruction16Bit >> 4) & 0x000F)];
	regs[((instruction16Bit >> 8) & 0x000F)] = (regX & regY);
}

void chip8cpu::XOR()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	uint8_t regY = regs[((instruction16Bit >> 4) & 0x000F)];
	regs[((instruction16Bit >> 8) & 0x000F)] = (regX ^ regY);
}

void chip8cpu::ADD_2()
{
	uint8_t OregX = regs[((instruction16Bit >> 8) & 0x000F)];

	uint8_t regY = regs[((instruction16Bit >> 4) & 0x000F)];

	regs[((instruction16Bit >> 8) & 0x000F)] += regY;

	if (regs[((instruction16Bit >> 8) & 0x000F)] < OregX) {
		regs[0xF] = 0x01;
	}
	else {
		regs[0xF] = 0x00;
	}
}

void chip8cpu::SUB()
{
	uint8_t OregX = regs[((instruction16Bit >> 8) & 0x000F)];

	uint8_t regY = regs[((instruction16Bit >> 4) & 0x000F)];

	regs[((instruction16Bit >> 8) & 0x000F)] += regY;

	if (regs[((instruction16Bit >> 8) & 0x000F)] > OregX) {
		regs[0xF] = 0x01;
	}
	else {
		regs[0xF] = 0x00;
	}
}

void chip8cpu::SHR()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];

	if (regX & 0x01 == 0x01) {
		regs[0xF] = 0x01;
	}
	else {
		regs[0xF] = 0x00;
	}
	regs[((instruction16Bit >> 8) & 0x000F)] /= 2;
}

void chip8cpu::SUBN()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	uint8_t regY = regs[((instruction16Bit >> 4) & 0x000F)];

	if (regY > regX) {
		regs[0xF] = 0x01;
	}
	else {
		regs[0xF] = 0x00;
	}

	regs[((instruction16Bit >> 4) & 0x000F)] -= regX;
}

void chip8cpu::SHL()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];

	if (regX & 0x80 == 0x80) {
		regs[0xF] = 0x01;
	}
	else {
		regs[0xF] = 0x00;
	}
	regs[((instruction16Bit >> 8) & 0x000F)] *= 2;
}

void chip8cpu::SNE_2()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	uint8_t regY = regs[((instruction16Bit >> 4) & 0x000F)];

	if (regX != regY) {
		pc += 2;
	}
}

void chip8cpu::LD_3()
{
	uint16_t val = regs[(instruction16Bit & 0x0FFF)];

	I = val;
}

void chip8cpu::JP_2()
{
	pc = (instruction16Bit & 0x0FFF) + regs[0x00];
}

void chip8cpu::RND() {
	uint8_t val = (instruction16Bit & 0x00FF);

	// seed the random generator
	srand(time(NULL));
	uint8_t ran = rand();

	regs[((instruction16Bit >> 8) & 0x000F)] = ran + val;
}

void chip8cpu::DRW()
{

}

