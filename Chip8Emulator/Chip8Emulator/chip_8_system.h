#pragma once
#include "bus.h"
#include "debugger.h"

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
private:
	/*
	*  e0 - path doesn't exist
	*  e1 - Program loaded is empty
	*/
	enum error_enum{ e0, e1};
	void start_cpu();
	void start_cpu_debug();
	void start_display_thread();
public:
	std::thread display_thread;
	void write_binary_to_memory(std::vector<uint8_t>);
public:
	std::unique_ptr<renderer> display_screen = nullptr;
	void start();
	void load_rom_from_path(std::string);
	void display_error(error_enum);
};
