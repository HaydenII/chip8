#include "chip_8_system.h"

namespace misc_functions {
	std::vector<uint8_t> LoadBinaryFile(const char* filename)
	{
		// open the file:
		std::basic_ifstream<uint8_t> file(filename, std::ios::binary);

		// read the data:
		return std::vector<uint8_t>((std::istreambuf_iterator<uint8_t>(file)), std::istreambuf_iterator<uint8_t>());
	}

	inline long long Get_Current_Time() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
}

void chip_8_system::start_display_thread()
{
	display_screen = std::unique_ptr<renderer>(new renderer);
	display_screen->ConnectBus(this);
	display_thread = std::thread(&renderer::render_loop, std::ref(*display_screen));
}

// Main CPU loop - clock the CPU 500 times a second (once every 2 ms)
void chip_8_system::start_cpu()
{
	long long lastTime = misc_functions::Get_Current_Time();
	long long CurrentTime;
	while (GetAsyncKeyState(VK_ESCAPE) == 0) {
		CurrentTime = misc_functions::Get_Current_Time();
		if (CurrentTime - lastTime >= 2) {
			cpu.clock();
			lastTime = CurrentTime;
		}
	}
}

void chip_8_system::load_rom(std::string instr)
{
	using namespace std::filesystem;

	path ReadPath;

	// If argument is not a full path append the string to the current path
	if (instr.find(':') && exists(instr)) {
		ReadPath = path(instr);
	}
	else {
		ReadPath = path(current_path() += instr);
	}

	// Load the chip8 binary
	std::vector<uint8_t> program = misc_functions::LoadBinaryFile(ReadPath.string().c_str());

	// Write the binary into memory
	uint16_t WritePtr = 0x200;
	for (auto& instr : program) {
		mem.write(WritePtr++, instr);
	}
}
