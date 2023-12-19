﻿#pragma once
#include "Vector2d.h"

struct MouseButton
{
	bool holdingDown;
	
	bool downThisFrame;
	bool upThisFrame;
};

struct Mouse
{
	Vector2d position;
	Vector2d wheel;
	
	MouseButton left;
	MouseButton right;
	
};

