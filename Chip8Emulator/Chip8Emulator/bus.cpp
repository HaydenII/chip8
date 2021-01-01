#include "bus.h"

Bus::Bus()
{
	cpu.ConnectBus(this);
	cpu.reset();
}

void Bus::get_keystate()
{
	if (GetKeyState('1') & 0x8000) {
		cpu.lastKeyPressed = cpu.k1;
		return;
	}
	if (GetKeyState('2') & 0x8000) {
		cpu.lastKeyPressed = cpu.k2;
		return;
	}
	if (GetKeyState('3') & 0x8000) {
		cpu.lastKeyPressed = cpu.k3;
		return;
	}
	if (GetKeyState('4') & 0x8000) {
		cpu.lastKeyPressed = cpu.kc;
		return;
	}
	if (GetKeyState('Q') & 0x8000) {
		cpu.lastKeyPressed = cpu.k4;
		return;
	}
	if (GetKeyState('W') & 0x8000) {
		cpu.lastKeyPressed = cpu.k5;
		return;
	}
	if (GetKeyState('E') & 0x8000) {
		cpu.lastKeyPressed = cpu.k6;
		return;
	}
	if (GetKeyState('R') & 0x8000) {
		cpu.lastKeyPressed = cpu.kd;
		return;
	}
	if (GetKeyState('A') & 0x8000) {
		cpu.lastKeyPressed = cpu.k7;
		return;
	}
	if (GetKeyState('S') & 0x8000) {
		cpu.lastKeyPressed = cpu.k8;
		return;
	}
	if (GetKeyState('D') & 0x8000) {
		cpu.lastKeyPressed = cpu.k9;
		return;
	}
	if (GetKeyState('F') & 0x8000) {
		cpu.lastKeyPressed = cpu.ke;
		return;
	}
	if (GetKeyState('Z') & 0x8000) {
		cpu.lastKeyPressed = cpu.ka;
		return;
	}
	if (GetKeyState('X') & 0x8000) {
		cpu.lastKeyPressed = cpu.k0;
		return;
	}
	if (GetKeyState('C') & 0x8000) {
		cpu.lastKeyPressed = cpu.kb;
		return;
	}
	if (GetKeyState('V') & 0x8000) {
		cpu.lastKeyPressed = cpu.kf;
		return;
	}
}
