#include "Layout.h"

Layout::Layout(Orientation orientation_, Vector2d size_, Vector2d origin_):
	orientation(orientation_), size(size_), origin(origin_)
{
}

Vector2d Layout::hex_to_pixel(Hexagon h)
{
	const Orientation& M = orientation;
	double x = (M.f0 * h.GetQ() + M.f1 * h.GetR()) * size.x;
	double y = (M.f2 * h.GetQ() + M.f3 * h.GetR()) * size.y;
	return Vector2d(x + origin.x, y + origin.y);
}

Vector2d Layout::hex_corner_offset(int corner) {
	double angle = 2.0 * M_PI * (orientation.start_angle + corner) / 6;
	return Vector2d(size.x * cos(angle), size.y * sin(angle));
}

void Layout::polygon_corners(Hexagon h, Vector2d corners[6]) {
	Vector2d center = hex_to_pixel(h);
	for (int i = 0; i < 6; i++) {
		Vector2d offset = hex_corner_offset(i);
		corners[i] = Vector2d{center.x + offset.x,center.y + offset.y};
	}
}