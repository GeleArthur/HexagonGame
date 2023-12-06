#pragma once
#include "GameEngine/GameEngine.h"
#include "Utils/Vector2d.h"


class Hexagon
{
	int q;
	int r;
	// int s; // Not needed.
	// float size;

public:
	Hexagon(int q, int r);
	Hexagon();
	void Set(int q, int r);

	int GetQ() const;
	int GetR() const;
	int GetS() const;

	int GetLength() const;
	int GetDistance(const Hexagon other) const;

	Hexagon operator+(const Hexagon other) const;
	Hexagon operator-(const Hexagon other) const;
	Hexagon operator*(int scaler) const;
};
