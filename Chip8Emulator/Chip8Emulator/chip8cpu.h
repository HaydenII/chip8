#pragma once
#include <cstdint>
#include <string>
#include <vector>

// For the random operation
#include <cstdlib>
#include <ctime>

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
	uint64_t display[32]{ 0 };
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
	uint8_t V0;
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
	uint8_t regs[0xF];

	// Address register
	uint16_t I;

	// Program counter
	uint16_t pc;

	// stack pointer
	uint8_t sp;
	uint16_t stack[16];
	inline void DecrementStackPointer();

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
	// Skip the next instruction if 8 bit val provided == in register referenced
	void SE();
	// Skip next instruction if 8 bit val provided != register referenced
	void SNE();
	// Skip next instructions if the two registers referenced in instruction are equal
	void SE_2();
	//Put the given value into register referenced
	void LD();
	// Add the value in register referenced to byte provided
	void ADD();
	// Set reg1 to value of reg2
	void LD_2();
	// Set reg1 to val after performing a bitwise OR on the values
	void OR();
	// Set reg1 to val after performing a bitwise AND on the values
	void AND();
	// Set reg1 to val after performing a bitwise XOR on the values
	void XOR();
	// Add reg2 to reg1 if result is greater than 8bits set vf to 1
	void ADD_2();
	// Subtract reg2 from reg1 if result wrapped around set vf to 1
	void SUB();
	// If the least sig bit of reg referenced is 1 set vf to 1 then devide reg ref by 2
	void SHR();
	// Subtract reg1 from reg2 if result will not wrapped around set vf to 1
	void SUBN();
	// If most sig bit of reg ref is 1 set vf to 1 then multiply reg by 2
	void SHL();
	// Compare reg1 to reg2 if != increment pc by 2
	void SNE_2();
	// Set the value of reg I to 12 bit val provided
	void LD_3();
	// Set the program counter to value provided + the value in reg0
	void JP_2();
	// Set reg ref to random number ANDed with value provided
	void RND();
	// Display, not using yet
	void DRW();
};

