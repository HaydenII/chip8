#include "main.h"


class create_rom {
private:
	std::vector<uint8_t> BIN_FILE;
	int chars_in_file = 0;

public:
	create_rom(std::string in_str) {
		write_string(in_str);
	}
	create_rom(char inchar) {
		write_char(inchar);
	}
	create_rom() {};

	void write_char(char inchar) {
		char chararry[]{ inchar };
		int number;
		if (isxdigit(inchar))
		{
			number = strtol(chararry, NULL, 16);
		}
		else {
			return;
		}

		// Set I to address in reg 0
		BIN_FILE.push_back(0xF0 + number);
		BIN_FILE.push_back(0x29);

		// Add x y pos to vectors 0 and 1
		BIN_FILE.push_back(0x60);
		BIN_FILE.push_back(0x0F + (chars_in_file++ * 5));
		BIN_FILE.push_back(0x61);
		BIN_FILE.push_back(0x0F);

		// Draw 5 bytes from location in I
		BIN_FILE.push_back(0xD0);
		BIN_FILE.push_back(0x15);
	}

	void write_string(std::string in_str) {
		for (auto& i : in_str) {
			write_char(i);
		}
	}

	std::vector<uint8_t> get_bin() {
		BIN_FILE.push_back(0x12);
		BIN_FILE.push_back(0x00);
		return BIN_FILE;
	}
};

int main() {
	std::string ROM_Path = "C:\\Users\\hayde\\Downloads\\test_opcode.ch8";

	chip_8_system chip8;
	chip8.load_rom_from_path(ROM_Path);
	chip8.start();

	return 1;
}
