#pragma once
#include <cstdint>
#include <string>
#include <vector>

class Bus;

/*
* ----
* 0x000-0x1FF = Chip 8 interpreter
* 0x200-0xFFF = RAM & Program ROM
* ----
*/
class chip8cpu
{
private:
	// Used to assemble a 16 bit address from two bytes
	uint16_t hi;
	uint16_t lo;
	uint16_t addr;

	uint16_t ReadNext2Bytes();
public:
	chip8cpu();
	~chip8cpu();

	void clock();
public:
	void write(uint16_t addr, uint8_t data);
	uint8_t read(uint16_t);

	// The data that has been fetched before the operation
	uint8_t fetched;
	// The opcode fetched at the beginning of the clock cycle
	uint16_t instruction16Bit;

	// Bus pointer
	Bus* bus;
	void ConnectBus(Bus* busptr);

	// Registers - Not put in an array for clarity - might change later
	uint8_t V1;
	uint8_t V2;
	uint8_t V3;
	uint8_t V4;
	uint8_t V5;
	uint8_t V6;
	uint8_t V7;
	uint8_t V8;
	uint8_t V9;
	uint8_t VA;
	uint8_t VB;
	uint8_t VC;
	uint8_t VD;
	uint8_t VE;
	uint8_t VF; // VF doubltes as a status flag for some instructions

	// Address register
	uint16_t I;

	// Program counter
	uint16_t pc;

	// stack pointer
	uint8_t sp;
	uint16_t stack[16];
	inline void DecrementStackPointer();

	// Array of pointers to the opcode functions
	struct INSTRUCTION {
		std::string name;
		uint8_t(chip8cpu::* operate)(void) = nullptr;
	};

	// All instructions are stored in order,
	std::vector<INSTRUCTION> ins;

	// Jump to code at address
	void SYS();
	// Clear the display - implement later
	void CLS();
	// Set pc to address at stack pointer
	void RET();
	// Set pc to address
	void JP();
	// Increment the stack pointer, put the current pc on the top of the stack
	// Then set pc to a new address
	void CALL();
	// Skip the next instruction
	void SE();
};

