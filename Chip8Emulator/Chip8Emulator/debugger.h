#pragma once
#include <iostream>
#include "cpu.h"

void print_cpu_state(chip8cpu* inCPU);
std::string instruction_name(uint8_t hi, uint8_t lo);