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

	class create_rom {
	private:
		std::vector<uint8_t> BIN_FILE;
		int chars_in_file = 0;

	public:
		create_rom(std::string in_str) {
			write_string(in_str);
		}
		create_rom(char inchar) {
			write_char(inchar);
		}
		create_rom() {};

		void write_char(char inchar) {
			char chararry[]{ inchar };
			int number;
			if (isxdigit(inchar))
			{
				number = strtol(chararry, NULL, 16);
			}
			else {
				return;
			}

			// Set I to address in reg 0
			BIN_FILE.push_back(0xF0 + number);
			BIN_FILE.push_back(0x29);

			// Add x y pos to vectors 0 and 1
			BIN_FILE.push_back(0x60);
			BIN_FILE.push_back(0x0F + (chars_in_file++ * 5));
			BIN_FILE.push_back(0x61);
			BIN_FILE.push_back(0x0F);

			// Draw 5 bytes from location in I
			BIN_FILE.push_back(0xD0);
			BIN_FILE.push_back(0x15);
		}

		void write_string(std::string in_str) {
			for (auto& i : in_str) {
				write_char(i);
			}
		}

		std::vector<uint8_t> get_bin() {
			uint16_t jpaddr = 0x1200 + chars_in_file * 8;
			BIN_FILE.push_back((jpaddr >> 8) & 0xFF);
			BIN_FILE.push_back(jpaddr & 0xFF);

			return BIN_FILE;
		}
	};
}

void chip_8_system::start_display_thread()
{
	display_screen = std::unique_ptr<renderer>(new renderer);
	display_screen->ConnectBus(this);
	display_thread = std::thread(&renderer::render_loop, std::ref(*display_screen));
}

void chip_8_system::start()
{
	start_display_thread();
	while (!display_screen->window.isOpen()) {
		Sleep(100);
	}
	start_cpu();
	display_thread.join();
}

// Main CPU loop - clock the CPU 500 times a second (once every 2 ms)
void chip_8_system::start_cpu()
{
	bool open_window = false;
	long long lastTime = misc_functions::Get_Current_Time();
	long long CurrentTime;
	while (!GetAsyncKeyState(VK_ESCAPE) && display_screen->window.isOpen()) {
		CurrentTime = misc_functions::Get_Current_Time();
		if (CurrentTime - lastTime >= 2) {
			cpu.clock();
			lastTime = CurrentTime;
		}
	}
}

// Main CPU loop - clock the CPU each time you press spacebar
// And print the entire state of the cpu in the console
void chip_8_system::start_cpu_debug()
{
	print_cpu_state(&cpu);
	while (true) {
 		if (GetAsyncKeyState(VK_SPACE) & 0x01) {
			cpu.clock();
			print_cpu_state(&cpu);
		}  
		if (GetAsyncKeyState(VK_ESCAPE) & 0x01) {
			break; 
		}
	}
}

void chip_8_system::load_rom_from_path(std::string instr)
{
	using namespace std::filesystem;

	path ReadPath;

	// If argument is not a full path append the string to the current path
	if (instr.find(':') && exists(instr)) {
		ReadPath = path(instr);
	}
	else {
		// Path doesn't exist
		display_error(e0);
	}

	// Load the chip8 binary
	std::vector<uint8_t> program = misc_functions::LoadBinaryFile(ReadPath.string().c_str());
	if (program.empty()) {
		display_error(e1);
	}

	write_binary_to_memory(program);
}

void chip_8_system::display_error(error_enum inerr)
{
	misc_functions::create_rom error_rom;
	cpu.reset();
	switch (inerr) {
	case(e0):
		error_rom.write_string("E0");
		break;

	case(e1):
		error_rom.write_string("E1");
		break;

	default:
		error_rom.write_string("F");
		break;
	}
	
	write_binary_to_memory(error_rom.get_bin());

}

void chip_8_system::write_binary_to_memory(std::vector<uint8_t> program)
{
	// Write the binary into memory
	uint16_t WritePtr = 0x200;
	for (auto& instr : program) {
		mem.write(WritePtr++, instr);
	}
}