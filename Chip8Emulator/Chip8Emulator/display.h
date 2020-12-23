#pragma once
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class display : public olc::PixelGameEngine
{
public:
	display();

public:
	bool OnUserCreate() override;

	bool OnUserUpdate(float fElapsedTime) override;
};
