#pragma once
#include <cstdint>

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
	uint8_t opcode;

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
	uint8_t SP;
	uint16_t stack[16];

	/*
		 Instructions
		 All instructions are 16bit
	*/

	// Jump to code at address
	void SYS();
	// Clear the display - implement later
	void CLS();
};

