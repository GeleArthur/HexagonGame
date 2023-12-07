#pragma once
#include "../GameEngine/GameEngine.h"
#include "../Utils/Vector2d.h"


class Hexagon
{
	int q;
	int r;

public:
	Hexagon(int q, int r);
	Hexagon();
	void Set(int q, int r);

	int GetQ() const;
	int GetR() const;
	int GetS() const;

	int GetLength() const;
	int GetDistance(const Hexagon other) const;
	static Hexagon Direction(int direction);
	Hexagon Neighbor(int direction) const;

	Hexagon operator+(const Hexagon other) const;
	Hexagon operator-(const Hexagon other) const;
	Hexagon operator*(int scaler) const;
	bool operator==(Hexagon other) const;
};
