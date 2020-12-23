#include "main.h"

class C8Display : public olc::PixelGameEngine
{
public:
	Bus* bus;
	C8Display()
	{
		sAppName = "Example";
	}
	void ConnectBus(Bus* InPtr) {
		bus = InPtr;
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Draw the display array connected to the bus
		for (int x = 0; x < 32; x++) {
			uint64_t TempDisplay = (*bus).display[x];
			for (int y = 0; y < 64; y++) {
				if ((TempDisplay >> (64 - y)) & 0x0000000000000001) {
					Draw(y, x, olc::WHITE);
				}
				else {
					Draw(y, x, olc::BLACK);
				}
			}
		}
		return true;
	}
};


int main() {
	Bus bus;
	bus.cpu.reset();

	bus.cpu.I = 0x0000;
	bus.cpu.write(0x200, 0xD0);
	bus.cpu.write(0x201, 0x05);
	bus.cpu.clock();

	C8Display game;
	game.ConnectBus(&bus);
	game.Construct(64, 32, 16, 16);
	game.Start();

	//uint8_t memptr = 0x0000;
	//std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	//std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	//std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	//std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	//std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;

	return 1;
}
