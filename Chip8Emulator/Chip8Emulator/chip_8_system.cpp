#include "chip_8_system.h"

namespace misc_functions {
	
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

/*
* Public
* ---
* Begins the cpu and the display threads
*/
void chip_8_system::start()
{
	start_display_thread();

	// Saves time by stopping render execution when it definitely won't be needed
	while (!display_screen->window.isOpen()) {
		Sleep(100);
	}
	start_cpu();

	// Closes the thread
	display_thread.join();
}

/*
* Loads the binary into the rom_loader object then read into memory
*/
void chip_8_system::load_rom(std::string inPath)
{
	// Loads the file as binary into binary_data member variable inside romloader
	error_enum loaderror = romloader.load_rom_from_path(inPath);

	if (loaderror != e0) { display_error(loaderror); return; }

	if (romloader.binary_data.empty()) { return; }

	write_binary_to_memory(&romloader.binary_data);

}


/*
* Private
* ---
* Begin the display loop
*/
void chip_8_system::start_display_thread()
{
	// The render thread requires a pointer to object
	display_screen = std::unique_ptr<renderer>(new renderer	);
	display_screen->ConnectBus(this);
	display_thread = std::thread(&renderer::render_loop, std::ref(*display_screen));
}

void chip_8_system::reset_system()
{
	cpu.reset();
	mem.wipe_memory();
	wipe_display_buffer();
}

/*
* Main CPU loop - clock the CPU 500 times a second(once every 2 ms)
*/
void chip_8_system::start_cpu()
{
	using clock = std::chrono::high_resolution_clock;
	using namespace std::literals;

	bool copypastetoggle = false;
	clock::time_point started = clock::now();
	clock::time_point target = started + 2ms;

	while (display_screen->window.isOpen()) {
		// Handle copy paste
		if (!copypastetoggle && GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(0x56)) {
			pasted_url = sf::Clipboard::getString();
			copypastetoggle = true;
		}
		else if (copypastetoggle && !(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(0x56))) { 
			copypastetoggle = false; 
		}

		// Close the window and shut the system down
		if (GetAsyncKeyState(VK_ESCAPE)) {
			display_screen->close_window();
		}

		// Run rom
		if (pasted_url != "" && GetAsyncKeyState(VK_RETURN)) {
			reset_system();

			std::cout << pasted_url.substr(1, pasted_url.size() -2) << std::endl;
			load_rom(pasted_url.substr(1, pasted_url.size() -2));

			pasted_url = "";
		}

		cpu.clock();
		std::this_thread::sleep_until(target);
		target += 2ms;
	}
}

/*
* Main CPU loop - clock the CPU each time you press spacebar
* And print the entire state of the cpu in the console
*/
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

	case(e2):
		error_rom.write_string("E2");
		break;

	case(e3):
		error_rom.write_string("E3");
		break;


	default:
		error_rom.write_string("F");
		break;
	}
	
	std::vector<uint8_t> errorRomBin = error_rom.get_bin();

	write_binary_to_memory(&errorRomBin);

}

void chip_8_system::write_binary_to_memory(std::vector<uint8_t>* program)
{
	// Write the binary into memory
	uint16_t WritePtr = 0x200;
	for (auto& instr : (*program)) {
		mem.write(WritePtr++, instr);
	}
}
