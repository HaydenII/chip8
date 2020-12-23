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
		{
			if (GetKey(olc::Key::SPACE).bPressed)
			{
				bus->cpu.clock();
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


int main() {
	Bus bus;
	bus.cpu.reset();

	/*
	* 1. Write sprite location to I
	* 2. Write draw location to register 0
	* 3. Write draw location to register 1
	* 4. Draw sprites
	*/
	uint16_t program[] = { 0xA019, 0x600F, 0x610F, 0xD015, 0xA005, 0x6009, 0x610F, 0xD015 };

	uint16_t WritePtr = 0x200;
	for (auto& instr : program) {
		uint8_t hi = instr >> 8;
		uint8_t lo = instr & 0x00FF;

		bus.mem.write(WritePtr++, hi);
		bus.mem.write(WritePtr++, lo);
	}

	//// INSTRUCTION 0
	//// Write sprite location to I register
	//bus.cpu.write(0x200, 0xA0);
	//bus.cpu.write(0x201, 0x19);

	//// INSTRUCTION 1
	//// Write sprite draw location to registers 0
	//bus.cpu.write(0x202, 0x60);
	//bus.cpu.write(0x203, 0x0F);

	//// Write sprite draw location to registers 1
	//bus.cpu.write(0x204, 0x61);
	//bus.cpu.write(0x205, 0x0F);

	//// Draw the sprites in registers 0 and 1
	//bus.cpu.write(0x206, 0xD0);
	//bus.cpu.write(0x207, 0x15);

	//// INSTRUCTION 2
	//bus.cpu.write(0x204, 0xA0);
	//bus.cpu.write(0x205, 0x05);

	//// INSTRUCTION 3
	//bus.cpu.regs[2] = 20;
	//bus.cpu.regs[3] = 20;
	//bus.cpu.write(0x206, 0xD2);
	//bus.cpu.write(0x207, 0x35);
	////bus.cpu.clock();

	C8Display game;
	game.ConnectBus(&bus);
	game.Construct(64, 32, 16, 16);
	game.Start();

	return 1;
}
