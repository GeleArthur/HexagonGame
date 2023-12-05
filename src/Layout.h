#pragma once
#include "Hexagon.h"
#include "Orientation.h"
#include "Utils/Vector2d.h"

struct Layout
{
	const Orientation orientation;
	const Vector2d size;
	Vector2d origin;
	Layout(Orientation orientation_, Vector2d size_, Vector2d origin_);

	Vector2d hex_to_pixel(Hexagon h);
	Vector2d hex_corner_offset(int corner) ;
	void polygon_corners(Hexagon h, Vector2d corners[6]) ;
};

