#include "main.h"
#include <iostream>


int main() {

	memory newmem;

	newmem.write(0xFFF, 0x010);

	std::cout << int(newmem.read(0x0FFF)) << std::endl;
	std::cout << int(newmem.read(0x0EFF)) << std::endl;

	return 1;
}
