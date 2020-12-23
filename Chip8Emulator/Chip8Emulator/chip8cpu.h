#pragma once
#include <cstdint>
#include <string>
#include <vector>

// For the random operation
#include <cstdlib>
#include <ctime>
#include <bitset>

class Bus;

/*
* ----
* 0x000-0x1FF = Chip 8 interpreter and font set
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

	void reset();

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

	// Registers - 0 .. F
	uint8_t regs[0xF] = {0};

	// Address register
	uint16_t I;

	// Timer registers
	uint8_t DelayTimer;
	uint8_t SoundTimer;

	// Program counter
	uint16_t pc;

	// stack pointer
	uint8_t sp;
	uint16_t stack[16];
	inline void DecrementStackPointer();

	// Jump to code at address
	void SYS();
	// Clear the display
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
	void DRW(); // FINISH LATER
	// Skip next instruction if the key associated with reg ref is pressed
	void SKP(); // FINISH LATER
	// Skip next instruction if the key associated with reg ref is not pressed
	void SKNP(); // FINISH LATER
	// Set reg ref to timer value
	void LD_4();
	// Pause execution until key is pressed then store key val in ref ref
	void LD_5(); // FINISH LATER
	// Set delay timer to reg ref
	void LD_6();
	// Set sound timer to reg ref
	void LD_7();
	// set register I to I + reg ref
	void ADD_3();
	// Set reg I to mem addr of sprite for digit located in reg ref
	void LD_8();
	// Take decimal value of reg ref and place the hundreds digit in memory at location in I, tens in i+1 ones at i+2
	void LD_9(); // FINISH LATER
	// Store register v0 .. vN in memory starting at location I
	void LD_10();
	// Read register v0.. vN from memory starting at location I
	void LD_11();
};

