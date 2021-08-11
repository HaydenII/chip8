#include "rom_loader.h"

rom_loader::rom_loader() : full_rom_path(""), rom_title("") { }

error_enum rom_loader::load_rom_from_path(std::string instr)
{
	using namespace std::filesystem;

	path ReadPath;

	// If argument is not a full path append the string to the current path
	if (instr.find(':') && exists(instr)) {
		ReadPath = path(instr);
	}
	else {
		return e1;
	}

	// Load the chip8 binary
	LoadBinaryFile(ReadPath.string().c_str());
	if (binary_data.empty()) {
		return e2;
	}
	else if (binary_data.size() % 2 == 1) {
		return e3;
	}

	return e0;
}

void rom_loader::LoadBinaryFile(const char* filename)
{
	// open the file:
	std::basic_ifstream<uint8_t> file(filename, std::ios::binary);

	// read the data:
	binary_data = std::vector<uint8_t>((std::istreambuf_iterator<uint8_t>(file)), std::istreambuf_iterator<uint8_t>());
}
