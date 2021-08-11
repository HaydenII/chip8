#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include "error_codes.h"

class rom_loader
{
public:
	std::string full_rom_path;
	std::string rom_title;
	rom_loader();
public:
	error_enum load_rom_from_path(std::string);
	std::vector<uint8_t> binary_data;
private:
	void LoadBinaryFile(const char*);
};