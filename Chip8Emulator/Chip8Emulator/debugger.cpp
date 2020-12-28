#include "debugger.h"
/*
* Reg 0	=
* Reg 1	=
* Reg 2	=
* Reg 3	=
* Reg 4	=
* Reg 5	=
* Reg 6	=
* Reg 7	=
* Reg 8	=
* Reg 9	=
* Reg A	=
* Reg B	=
* Reg C	=
* Reg D	=
* Reg E	=
* Reg F	=
* 
* I		=
* 
* PC	=
* SP	=
* 
* Current Instruction
* 6012	= LD
*/

std::string instruction_name(uint8_t hi, uint8_t lo) {
	uint16_t instruction16Bit = (hi << 8) | lo;

	// Highest order bits
	switch (instruction16Bit >> 12) {
	case (0x0000):
		// Lowest order bits
		switch (instruction16Bit & 0x000F) {
		case (0):
			return "CLS()";
			break;
		case (0xE):
			return "RET()";
			break;
		default:
			return "SYS()";
			break;
		}
	case (0x1):
		return "JP()";
		break;
	case (0x2):
		return "CALL()";
		break;
	case (0x3):
		return "SE()";
		break;
	case (0x4):
		return "SNE()";
		break;
	case (0x5):
		return "SE_2()";
		break;
	case (0x6):
		return "LD()";
		break;
	case (0x7):
		return "ADD()";
		break;
	case (0x8):
		// Lowest order bits
		switch (instruction16Bit & 0x000F) {
		case (0x0):
			return "LD_2()";
			break;
		case (0x1):
			return "OR()";
			break;
		case (0x2):
			return "AND()";
			break;
		case (0x3):
			return "XOR()";
			break;
		case (0x4):
			return "ADD_2()";
			break;
		case (0x5):
			return "SUB()";
			break;
		case (0x6):
			return "SHR()";
			break;
		case (0x7):
			return "SUBN()";
			break;
		case (0xE):
			return "SHL()";
			break;
		}
	case (0x9):
		return "SNE_2()";
		break;
	case (0xA):
		return "LD_3()";
		break;
	case (0xB):
		return "JP_2()";
		break;
	case (0xC):
		return "RND()";
		break;
	case (0xD):
		return "DRW()";
		break;
	case (0xE):
		// Lowest order bits
		switch (instruction16Bit & 0x000F) {
		case(0xE):
			return "SKP()";
			break;
		case(0x1):
			return "SKNP()";
			break;
		}
	case (0xF):
		// 2 lowest order bits
		switch (instruction16Bit & 0x00FF) {
		case(0x07):
			return "LD_4()";
			break;
		case(0x0A):
			return "LD_5()";
			break;
		case(0x15):
			return "LD_6()";
			break;
		case(0x18):
			return "LD_7()";
			break;
		case(0x1E):
			return "ADD_3()";
			break;
		case(0x29):
			return "LD_8()";
			break;
		case(0x33):
			return "LD_9()";
			break;
		case(0x55):
			return "LD_10()";
			break;
		case(0x65):
			return "LD_11()";
			break;
		}
	default:
		return " | No matching function | ";
		break;
	}
}

void print_cpu_state(chip8cpu* inCPU) {
	using namespace std;

	cout << "------------------------------------------" << endl;

	for (int i = 0; i < 16; i++) {
		cout << "Reg " << i <<	"	= " << hex << (int)inCPU->regs[i] << endl;
	}

	cout << endl << "I " << "	= " << inCPU->I << endl;
	cout << endl << "PC " << "	= " << inCPU->pc << endl;
	cout << "SP " << "	= ";
	for (int i = 0; i < 16; i++) {
		cout << " " << inCPU->stack[i] << ", ";
	}
	cout << "\nSP index" << "= " << (int)inCPU->sp << endl;

	cout << "\nDelay Timer" << "= " << (int)inCPU->DelayTimer << endl;
	cout << "\nSound Timer" << "= " << (int)inCPU->SoundTimer << endl;

	uint16_t lo = inCPU->read(inCPU->pc - 1);
	uint16_t hi = inCPU->read(inCPU->pc - 2);
	uint16_t PI = (hi << 8) | lo;

	uint16_t lo2 = inCPU->read(inCPU->pc + 1);
	uint16_t hi2 = inCPU->read(inCPU->pc);
	uint16_t PI2 = (hi2 << 8) | lo2;

	cout << endl << endl << "Previous Instruction " << endl;
	cout << hex << PI << " = " << instruction_name(hi, lo) << endl;
	cout << endl << "Next Instruction " << endl;
	cout << hex << PI2 << " = " << instruction_name(hi2, lo2) << endl;
	cout << "------------------------------------------" << endl << endl;
}