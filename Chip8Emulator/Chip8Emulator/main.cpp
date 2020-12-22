#include "main.h"

int main() {

	Bus bus;

	bus.cpu.write(0xFFF, 0x010);

	std::cout << int(bus.cpu.read(0x0FFF)) << std::endl;
	std::cout << int(bus.cpu.read(0x0EFF)) << std::endl;

	return 1;
}
