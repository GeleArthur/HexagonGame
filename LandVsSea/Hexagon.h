#pragma once
#include "utils.h"

struct Hexagon
{
	int q;
	int r;
	int s; // Not needed.
	float size;

	Point2f GetCoords();
	void DrawHexagons();
};
