#include "bus.h"

Bus::Bus()
{
	cpu.ConnectBus(this);
}

Bus::~Bus()
{
}
