#include "main.h"

int main() {

	std::string ROM_Path = "C:\\Users\\hayde\\Downloads\\octojam6title.ch8";

	chip_8_system chip8;

	chip8.load_rom(ROM_Path);

 	chip8.start_display_thread();

	chip8.start_cpu();

	chip8.display_thread.join();

	return 1;
}
#
