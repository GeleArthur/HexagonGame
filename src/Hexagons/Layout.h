#pragma once
#include "Hexagon.h"
#include "Orientation.h"
#include "../Utils/Vector2d.h"

struct Layout
{
	const Orientation orientation;
	Vector2d size;
	Vector2d origin;
	Layout(Orientation orientation_, Vector2d size_, Vector2d origin_);

	Vector2d HexToPixel(Hexagon h) const;
	Vector2d HexCornerOffset(int corner) const;
	void PolygonCorners(Hexagon h, Vector2d corners[6]) const;
	Hexagon PixelToHex(Vector2d point) const;
	Vector2d GetDistanceBetweenHexPointUp() const;
};

