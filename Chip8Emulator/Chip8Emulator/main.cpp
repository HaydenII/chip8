#include "main.h"

class C8Display : public olc::PixelGameEngine
{
public:
	Bus* bus;
	C8Display()
	{
		sAppName = "Chip 8 Emulator";
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
		if (IsFocused())
		{ // 0xE0A2
			if (/*GetKey(olc::Key::SPACE).bHeld || */GetKey(olc::Key::SPACE).bPressed)
			{
				bus->cpu.clock();
				print_cpu_state(&bus->cpu);
			}
			if (GetKey(olc::Key::CTRL).bHeld)
			{
				bus->cpu.clock();
				print_cpu_state(&bus->cpu);
			}
		}

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

std::vector<uint8_t> readFile(const char* filename)
{
	// open the file:
	std::basic_ifstream<uint8_t> file(filename, std::ios::binary);

	// read the data:
	return std::vector<uint8_t>((std::istreambuf_iterator<uint8_t>(file)), std::istreambuf_iterator<uint8_t>());
}



int main() {
	Bus bus;
	bus.cpu.reset();

	/*
	* 1. Write sprite location to I
	* 2. Write draw location to register 0
	* 3. Write draw location to register 1
	* 4. Draw sprites
	*/
	// Draw two characters
	//uint16_t program[] = { 0xA0, 0x19, 0x60, 0x0F, 0x61, 0x0F, 0xD0, 0x15, 0xA0, 0x05, 0x60, 0x09, 0x61, 0x0F, 0xD0, 0x15 };

	// Move the char 5 diagonally down the screen
	//uint16_t program[] = { 0xA0, 0x19,   0x60, 0x00,   0x61, 0x00,   0x70, 0x01,   0x71, 0x01,   0xD0, 0x15,   0x00, 0xE0,   0x12, 0x06};

	
	std::vector<uint8_t> program = readFile("C:\\Users\\hayde\\Downloads\\c8_test.c8");

	uint16_t WritePtr = 0x200;
	for (auto& instr : program) {
		bus.mem.write(WritePtr++, instr);
	}

	print_cpu_state(&bus.cpu);

	C8Display game;
	game.ConnectBus(&bus);
	game.Construct(64, 32, 16, 16);
	game.Start();

	return 1;
}
