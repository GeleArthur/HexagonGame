#pragma once
#include "Hexagon.h"
#include "Orientation.h"
#include "../Utils/Vector2d.h"

struct Layout
{
	const Orientation orientation;
	const Vector2d size;
	Vector2d origin;
	Layout(Orientation orientation_, Vector2d size_, Vector2d origin_);

	Vector2d HexToPixel(Hexagon h);
	Vector2d HexCornerOffset(int corner) ;
	void PolygonCorners(Hexagon h, Vector2d corners[6]) ;
	Hexagon PixelToHex(Vector2d point);
};

