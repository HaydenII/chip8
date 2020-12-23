#include "chip8cpu.h"
#include "bus.h"

chip8cpu::chip8cpu()
{
}

chip8cpu::~chip8cpu()
{
}

void chip8cpu::reset()
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

	// Point to beginning of program
	pc = 0x200;

	// Reset the registers
	for (auto& reg : regs) {
		reg = 0x00;
	}
	I = 0x0000;

	DelayTimer = 60;
	SoundTimer = 60;

	// Reset the stack
	sp = 0;
	for (auto& layer : stack) {
		layer = 0x0000;
	}
}

void chip8cpu::clock()
{
	hi = read(pc);
	pc++;
	lo = read(pc);
	pc++;

	instruction16Bit = (hi << 8) | lo;

	switch (hi >> 4) {
	case (0):
		switch (hi >> 4) {
		case (0):
			// code block
			break;
		}
	case (1):
		JP();
		break;
	case (2):
		CALL();
		break;
	case (3):
		SE();
		break;
	case (4):
		SNE();
		break;
	case (5):
		SE_2();
		break;
	case (6):
		LD();
		break;
	case (7):
		ADD();
		break;
	case (8):
		switch (lo & 0x0F) {
		case (0x00):
			LD_2();
			break;
		case (0x01):
			OR();
			break;
		case (0x02):
			AND();
			break;
		case (0x03):
			XOR();
			break;
		case (0x04):
			ADD_2();
			break;
		case (0x05):
			SUB();
			break;
		case (0x06):
			SHR();
			break;
		case (0x07):
			SUBN();
			break;
		case (0x0E):
			SHL();
			break;
		}
	case (9):
		SNE_2();
		break;
	case (0xA):
		LD_3();
		break;
	case (0xB):
		JP_2();
		break;
	case (0xC):
		RND();
		break;
	case (0xD):
		DRW();
		break;
	case (0xE):
		switch (lo & 0x0F) {
		case(0x0E):
			SKP();
			break;
		case(0x01):
			SKNP();
			break;
		}
	case (0xF):
		switch (lo) {
		case(0x07):
			LD_4();
			break;
		case(0x0A):
			LD_5();
			break;
		case(0x15):
			LD_6();
			break;
		case(0x18):
			LD_7();
			break;
		case(0x1E):
			ADD_3();
			break;
		case(0x29):
			LD_8();
			break;
		case(0x33):
			LD_9();
			break;
		case(0x55):
			LD_10();
			break;
		case(0x65):
			LD_11();
			break;
		}
		break;
	default:
		break;
	}
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
	// Position on the screen is stored in the registers referenced in the instruction
	uint8_t x = regs[(instruction16Bit >> 8) & 0x00F]; // x Coordinate
	uint8_t y = regs[(instruction16Bit >> 4) & 0x00F]; // y Coordinate

	// Starting memory location
	uint16_t sMemLoc = I;
	uint8_t BytesToRead = (instruction16Bit & 0x000F);

	for (int i = 0; i < BytesToRead; i++) {
		uint64_t sprite_row = read(I + i);
		bus->display[i] ^= sprite_row;
	}
}

void chip8cpu::SKP()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];
}

void chip8cpu::SKNP()
{
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

