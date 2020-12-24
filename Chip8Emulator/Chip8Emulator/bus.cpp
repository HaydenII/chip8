#include "bus.h"

Bus::Bus()
{
	cpu.ConnectBus(this);
}

Bus::~Bus()
{
}

void Bus::get_keystate()
{
	if (GetKeyState('1') & 0x8000) {
		cpu.lastKeyPressed = cpu.k1;
	}
	if (GetKeyState('2') & 0x8000) {
		cpu.lastKeyPressed = cpu.k2;
	}
	if (GetKeyState('3') & 0x8000) {
		cpu.lastKeyPressed = cpu.k3;
	}
	if (GetKeyState('4') & 0x8000) {
		cpu.lastKeyPressed = cpu.k4;
	}
	if (GetKeyState('5') & 0x8000) {
		cpu.lastKeyPressed = cpu.k5;
	}
	if (GetKeyState('6') & 0x8000) {
		cpu.lastKeyPressed = cpu.k6;
	}
	if (GetKeyState('7') & 0x8000) {
		cpu.lastKeyPressed = cpu.k7;
	}
	if (GetKeyState('8') & 0x8000) {
		cpu.lastKeyPressed = cpu.k8;
	}
	if (GetKeyState('9') & 0x8000) {
		cpu.lastKeyPressed = cpu.k9;
	}
	if (GetKeyState('A') & 0x8000) {
		cpu.lastKeyPressed = cpu.ka;
	}
	if (GetKeyState('B') & 0x8000) {
		cpu.lastKeyPressed = cpu.kb;
	}
	if (GetKeyState('C') & 0x8000) {
		cpu.lastKeyPressed = cpu.kc;
	}
	if (GetKeyState('D') & 0x8000) {
		cpu.lastKeyPressed = cpu.kd;
	}
	if (GetKeyState('E') & 0x8000) {
		cpu.lastKeyPressed = cpu.ke;
	}
	if (GetKeyState('F') & 0x8000) {
		cpu.lastKeyPressed = cpu.kf;
	}
}
