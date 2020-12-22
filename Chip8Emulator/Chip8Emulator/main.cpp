#include "main.h"

//class C8Display : public olc::PixelGameEngine
//{
//public:
//	C8Display()
//	{
//		sAppName = "Example";
//	}
//
//public:
//	bool OnUserCreate() override
//	{
//		// Called once at the start, so create things here
//		return true;
//	}
//
//	bool OnUserUpdate(float fElapsedTime) override
//	{
//		// called once per frame
//		for (int x = 0; x < ScreenWidth(); x++)
//			for (int y = 0; y < ScreenHeight(); y++)
//				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
//		return true;
//	}
//};


int main() {
	Bus bus;
	bus.cpu.initialise();

	uint8_t memptr = 0x0000;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;
	std::cout << std::bitset<8>(bus.cpu.read(memptr++)) << std::endl;

	//C8Display game;
	//game.Construct((64 * 4), (32 * 4), 4, 4);
	//game.Start();

	return 1;
}
