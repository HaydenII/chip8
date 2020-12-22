#include "main.h"

int main() {
	Bus bus;
	bus.cpu.initialise();

	//bus.cpu.write(0xFFF, 0x010);

	uint8_t memptr = 0x0000;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;

	return 1;
}
