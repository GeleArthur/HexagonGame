#pragma once
#include "GameEngine/GameEngine.h"
#include "Utils/Vector2d.h"


struct Hexagon
{
	Hexagon(int q, int r);
	Hexagon();
	
	int q;
	int r;
	// int s; // Not needed.
	// float size;



	Vector2d GetCoords();
	void DrawHexagons();
	int GetS();
};
