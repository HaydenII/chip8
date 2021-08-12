#pragma once
#include "bus.h"
#include "debugger.h"
#include "error_codes.h"

#include <memory>

#include <iterator>
#include <sstream>
#include <fstream>
#include <thread>
#include <filesystem>
#include <Windows.h>
#include <chrono>

class chip_8_system : public Bus
{
public:
	void start();
	void load_rom(std::string);

	std::string pasted_url = "";

private:
	/*
	* initialiser functions
	*/
	void start_cpu();
	void start_cpu_debug();
	void start_display_thread();

	/*
	* pointer to display. 
	* 
	* Not in bus because it's not part
	* of the internal system. Putting it in the chip_8 class
	* is analagous to plugging it into the system
	* 
	* The display thread requires a pointer to object
	*/
	std::unique_ptr<renderer> display_screen = nullptr;
	std::thread display_thread;

	/*
	* Misc.
	*/
	void display_error(error_enum);

	void write_binary_to_memory(std::vector<uint8_t>*);
};
