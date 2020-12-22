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
}

chip8cpu::~chip8cpu()
{
}

void chip8cpu::initialise()
{
	/*
	* LOADING FONSET IN STARTING AT 0X0000
	*/
	// Load fontset
	uint16_t MemPtr = 0x0000;
	uint8_t font_set[0x10][5] = {
		{0xF0, 0x90, 0x90, 0x90, 0xF0},
		{0x20, 0x60, 0x20, 0x20, 0x70},
		{0xF0, 0x10, 0xF0, 0x80, 0xF0},
		{0xF0, 0x10, 0xF0, 0x10, 0xF0},
		{0x90, 0x90, 0xF0, 0x10, 0x10},
		{0xF0, 0x80, 0xF0, 0x10, 0xF0},
		{0xF0, 0x80, 0xF0, 0x90, 0xF0},
		{0xF0, 0x10, 0x20, 0x40, 0x40},
		{0xF0, 0x90, 0xF0, 0x90, 0xF0},
		{0xF0, 0x90, 0xF0, 0x10, 0xF0},
		{0xF0, 0x90, 0xF0, 0x90, 0x90},
		{0xE0, 0x90, 0xE0, 0x90, 0xE0},
		{0xF0, 0x80, 0x80, 0x80, 0xF0},
		{0xE0, 0x90, 0x90, 0x90, 0xE0},
		{0xF0, 0x80, 0xF0, 0x80, 0xF0},
		{0xF0, 0x80, 0xF0, 0x80, 0x80}
	};
	for (auto& charactar : font_set) {
		for (auto& byte : charactar) {
			bus->mem.write(MemPtr++, byte);
		}
	}
}

void chip8cpu::clock()
{
	instruction16Bit = ReadNext2Bytes();
	uint8_t lb = instruction16Bit & 0x0001;
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
	// Starting memory location
	uint16_t sMemLoc = I;
	uint8_t BytesToRead = (instruction16Bit & 0x000F);
}

void chip8cpu::SKP()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
}

void chip8cpu::LD_4()
{
	regs[((instruction16Bit >> 8) & 0x000F)] = DelayTimer;
}

void chip8cpu::LD_5()
{
	// Loop and check for key press
	// break from loop when key pressed
	// store key val in reg ref

	uint8_t KeyVal = 0;
	regs[((instruction16Bit >> 8) & 0x000F)] = KeyVal;
}

void chip8cpu::LD_6()
{
	DelayTimer = regs[((instruction16Bit >> 8) & 0x000F)];
}

void chip8cpu::LD_7()
{
	SoundTimer = regs[((instruction16Bit >> 8) & 0x000F)];
}

void chip8cpu::ADD_3()
{
	I += (uint16_t)regs[((instruction16Bit >> 8) & 0x000F)];
}

void chip8cpu::LD_8()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	
	I = (uint16_t)(regX * 0X05);
}

void chip8cpu::LD_9()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	write(I, (regX & 0x03));
	write(I + 1, (regX & 0x02));
	write(I + 2, (regX & 0x01));
}

void chip8cpu::LD_10()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	for (int j = 0; j <= regX; j++) {
		write(I+=j, regs[j]);
	}
}

void chip8cpu::LD_11()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
	for (int j = 0; j <= regX; j++) {
		regs[I += j] = read(I += j);
	}
}

