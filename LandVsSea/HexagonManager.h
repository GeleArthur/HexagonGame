#pragma once
#include "Hexagon.h"

struct HexagonManager
{
	HexagonManager();
	~HexagonManager();
	void SetData(const int width, const int height, const float size);

	Hexagon** data;
	int width;
	int height;
};
