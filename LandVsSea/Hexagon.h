#pragma once
#include "utils.h"

struct Hexagon
{
	Hexagon();
	void SetUp(int q, int r, float size);
	
	int q;
	int r;
	// int s; // Not needed.
	float size;

	void GetCoords(Point2f& out);
	void DrawHexagons();
};
