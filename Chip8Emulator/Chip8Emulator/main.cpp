#include "main.h"
#include "bus.h"

int main() {

	std::string ROM_Path = "C:\\Users\\hayde\\Downloads\\chip8-master\\roms\\Tetris [Fran Dachille, 1991].ch8";

	chip_8_system chip8;
	chip8.load_rom(ROM_Path);
	chip8.start();
	
	
	return 1;
}
