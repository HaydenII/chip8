#pragma once
#include "bus.h"

#include <memory>

#include <iterator>
#include <sstream>
#include <fstream>
#include <thread>
#include <filesystem>

class chip_8_system : public Bus
{
public:
	std::thread display_thread;
public:
	std::unique_ptr<renderer> display_screen = nullptr;
	void start_cpu();
	void start_display_thread();
	void load_rom(std::string);
};
