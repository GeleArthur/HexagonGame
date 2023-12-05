#pragma once

struct Mouse
{
	float x;
	float y;
	bool holdingDown;
	
	bool downThisFrame;
	bool upThisFrame;
};