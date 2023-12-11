#pragma once
#include "Vector2d.h"

struct Rect
{
	Rect();
	explicit Rect( float left, float top, float width, float height );
	
	float left;
	float top;
	float width;
	float height;

	bool PointInRect(Vector2d point) const;
};
