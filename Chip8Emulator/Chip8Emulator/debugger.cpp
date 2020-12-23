#include "decompiler.h"
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
	switch (hi >> 4) {
	case (0):
		switch (hi >> 4) {
		case (0):
			// code block
			break;
		}
	case (1):
		return "JP";
		break;
	case (2):
		return "CALL";
		break;
	case (3):
		return "SE";
		break;
	case (4):
		return "SNE";
		break;
	case (5):
		return "SE_2";
		break;
	case (6):
		return "LD";
		break;
	case (7):
		return "ADD";
		break;
	case (8):
		switch (lo & 0x0F) {
		case (0x00):
			return "LD_2";
			break;
		case (0x01):
			return "OR";
			break;
		case (0x02):
			return "AND";
			break;
		case (0x03):
			return "XOR";
			break;
		case (0x04):
			return "ADD_2";
			break;
		case (0x05):
			return "SUB";
			break;
		case (0x06):
			return "SHR";
			break;
		case (0x07):
			return "SUBN";
			break;
		case (0x0E):
			return "SHL";
			break;
		}
	case (9):
		return "SNE_2";
		break;
	case (0xA):
		return "LD_3";
		break;
	case (0xB):
		return "JP_2";
		break;
	case (0xC):
		return "RND";
		break;
	case (0xD):
		return "DRW";
		break;
	case (0xE):
		switch (lo & 0x0F) {
		case(0x0E):
			return "SKP";
			break;
		case(0x01):
			return "SKNP";
			break;
		}
	case (0xF):
		switch (lo) {
		case(0x07):
			return "LD_4";
			break;
		case(0x0A):
			return "LD_5";
			break;
		case(0x15):
			return "LD_6";
			break;
		case(0x18):
			return "LD_7";
			break;
		case(0x1E):
			return "ADD_3";
			break;
		case(0x29):
			return "LD_8";
			break;
		case(0x33):
			return "LD_9";
			break;
		case(0x55):
			return "LD_10";
			break;
		case(0x65):
			return "LD_11";
			break;
		}
		break;
	default:
		break;
	}
}

void print_cpu_state(chip8cpu* inCPU) {
	using namespace std;

	cout << "------------------------------------------" << endl;

	for (int i = 0; i < 16; i++) {
		cout << "Reg " << i <<	"	= " << int(inCPU->regs[i]) << endl;
	}

	cout << endl << "I " << "	= " << inCPU->I << endl;
	cout << endl << "PC " << "	= " << inCPU->pc << endl;
	cout << "SP " << "	= " << inCPU->sp << endl;

	uint16_t lo = inCPU->read(inCPU->pc - 1);
	uint16_t hi = inCPU->read(inCPU->pc - 2);
	uint16_t PI = (hi << 8) | lo;

	uint16_t lo2 = inCPU->read(inCPU->pc + 1);
	uint16_t hi2 = inCPU->read(inCPU->pc);
	uint16_t PI2 = (hi2 << 8) | lo2;

	cout << endl << "Previous Instruction " << endl;
	cout << hex << PI << " = " << instruction_name(hi, lo) << endl;
	cout << endl << "Next Instruction " << endl;
	cout << hex << PI2 << " = " << instruction_name(hi2, lo2) << endl;
	cout << "------------------------------------------" << endl << endl;
}