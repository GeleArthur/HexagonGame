#pragma once
#include "Vector2d.h"

struct Mouse
{
	Vector2d position;
	bool holdingDown;
	
	bool downThisFrame;
	bool upThisFrame;
};