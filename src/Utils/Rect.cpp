#include "Rect.h"

Rect::Rect(): left(0), top(0), width(0), height(0)
{
	
}

Rect::Rect(float left, float top, float width, float height) : left(left), top(top), width(width), height(height)
{
	
}

bool Rect::PointInRect(const Vector2d point) const
{
	return point.x > left && point.x < left+width &&
		   point.y > top && point.y < top+height;
}