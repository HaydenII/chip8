#include "chip8cpu.h"
#include "bus.h"

chip8cpu::chip8cpu()
{
	srand(time(NULL));
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
	sp = 0xFF;

	// Reset the registers
	for (auto& reg : regs) {
		reg = 0x00;
	}
	I = 0x0000;

	DelayTimer = 1;
	SoundTimer = 1;

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

	// Highest order bits
	switch (instruction16Bit >> 12) {
	case (0x0000):
		// Lowest order bits
		switch (instruction16Bit & 0x000F) {
		case (0):
			CLS();
			break;
		case (0xE):
			RET();
			break;
		default:
			SYS();
			break;
		}
		break;
	case (0x1):
		JP();
		break;
	case (0x2):
		CALL();
		break;
	case (0x3):
		SE();
		break;
	case (0x4):
		SNE();
		break;
	case (0x5):
		SE_2();
		break;
	case (0x6):
		LD();
		break;
	case (0x7):
		ADD();
		break;
	case (0x8):
		// Lowest order bits
		switch (instruction16Bit & 0x000F) {
		case (0x0):
			LD_2();
			break;
		case (0x1):
			OR();
			break;
		case (0x2):
			AND();
			break;
		case (0x3):
			XOR();
			break;
		case (0x4):
			ADD_2();
			break;
		case (0x5):
			SUB();
			break;
		case (0x6):
			SHR();
			break;
		case (0x7):
			SUBN();
			break;
		case (0xE):
			SHL();
			break;
		}
		break;
	case (0x9):
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
		// Lowest order bits
		switch (instruction16Bit & 0x000F) {
		case(0xE):
			SKP();
			break;
		case(0x1):
			SKNP();
			break;
		}
		break;
	case (0xF):
		// 2 lowest order bits
		switch (instruction16Bit & 0x00FF) {
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

	// Get the time difference between this clock and last clock
	// If diff > 16.6 ms tick the timers
	long long CurrentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (CurrentTime - LastTimeUpdate >= 16.6) {
		DelayTimer--;
		SoundTimer--;
		LastTimeUpdate = CurrentTime;
	}

	if (SoundTimer == 1) {
		// Beep
	}
}

void chip8cpu::decrementDelay()
{

}

void chip8cpu::decrementSound()
{
	SoundTimer--;
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

void chip8cpu::SYS()
{
	pc = (instruction16Bit & 0x0FFF);
}

void chip8cpu::CLS()
{
	for (auto& pixel : bus->display) {
		pixel = 0;
	}
}

void chip8cpu::RET()
{
	pc = stack[sp--];
}

void chip8cpu::JP()
{
	pc = (instruction16Bit & 0x0FFF);
}

void chip8cpu::CALL() 
{
	stack[++sp] = pc;
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
	uint8_t XVal = regs[((instruction16Bit >> 8) & 0x000F)];

	uint8_t YVal = regs[((instruction16Bit >> 4) & 0x000F)];

	regs[((instruction16Bit >> 8) & 0x000F)] -= YVal;

	if (XVal > YVal) {
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
	uint8_t XVal = regs[((instruction16Bit >> 8) & 0x000F)];
	uint8_t YVal = regs[((instruction16Bit >> 4) & 0x000F)];

	if (YVal > XVal) {
		regs[0xF] = 0x01;
	}
	else {
		regs[0xF] = 0x00;
	}

	regs[((instruction16Bit >> 8) & 0x000F)] = YVal - XVal;
}

void chip8cpu::SHL()
{
	uint8_t regX = regs[((instruction16Bit >> 8) & 0x000F)];

	if ((regX & 0x80) == 0x80) {
		regs[0xF] = 0x1;
	}
	else {
		regs[0xF] = 0x0;
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
	I = (instruction16Bit & 0x0FFF);
}

void chip8cpu::JP_2()
{
	pc = (instruction16Bit & 0x0FFF) + regs[0x00];
}

void chip8cpu::RND() {
	uint8_t val = (instruction16Bit & 0x00FF);

	// seed the random generator
	uint8_t ran = rand();
	auto res = (ran & val);
	regs[((instruction16Bit >> 8) & 0x000F)] = (ran & val);
}

void chip8cpu::DRW()
{
	// Starting memory location
	uint8_t BytesToRead = (instruction16Bit & 0x000F);

	// Position on the screen is stored in the registers referenced in the instruction
	uint8_t x = regs[(instruction16Bit >> 8) & 0x00F]; // x Coordinate
	uint8_t y = regs[(instruction16Bit >> 4) & 0x00F]; // y Coordinate

	bool OW = false;
	for (int i = 0; i < BytesToRead; i++) {
		uint64_t sprite_row = read(I + i);
		// shifting by (56-x) doesn't work when x is larger than 56 and effectively offsets it to the right
		sprite_row = sprite_row << (56);
		sprite_row = sprite_row >> (x);
		
		// Check if the display is going to have a 1 turned to a 0
		if (!OW) {
			// activate get the zeroes in the difference between the screen row and the sprite row
			auto zeroes_in_diff = ~(bus->display[y] ^ sprite_row);
			// & the screenrow with the flipped zeroes on the resulting row
			// Any matching bits means a 1 in the display row flipped to a 0
			auto flipped_ones = bus->display[y] & zeroes_in_diff;
			if (flipped_ones) { OW = true; }
		}
		

		bus->display[y++] ^= sprite_row;
	}
	if (OW) { regs[0xF] = 0x1; }
	else { regs[0xF] = 0x0; }
}

void chip8cpu::SKP()
{
	bus->get_keystate();
	if (lastKeyPressed == Key(((instruction16Bit >> 8) & 0x000F))) {
		pc += 2;
	}
	lastKeyPressed = k;
}

void chip8cpu::SKNP()
{
	bus->get_keystate();
	if (lastKeyPressed != Key(((instruction16Bit >> 8) & 0x000F))) {
		pc+=2;
	}
	lastKeyPressed = k;
}

void chip8cpu::LD_4()
{
	regs[((instruction16Bit >> 8) & 0x000F)] = DelayTimer;
}

void chip8cpu::LD_5()
{
	while (lastKeyPressed == k) {
		bus->get_keystate();
		//std::this_thread::sleep_for(std::chrono::milliseconds(60));
	}
	regs[((instruction16Bit >> 8) & 0x000F)] = lastKeyPressed;
	lastKeyPressed = k;
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
	for (int j = 0; j < ((instruction16Bit >> 8) & 0x000F); j++) {
		regs[j] = read(I += j);
	}
}

