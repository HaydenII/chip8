#include "main.h"

int main() {
	std::string ROM_Path = "C:\\Users\\hayde\\Downloads\\test_opcyode.ch8";

	chip_8_system chip8;
	chip8.load_rom_from_path(ROM_Path);
	chip8.start();

	return 1;
}
