#include "Layout.h"

Layout::Layout(Orientation orientation_, Vector2d size_, Vector2d origin_):
	orientation(orientation_), size(size_), origin(origin_)
{
}

Vector2d Layout::HexToPixel(Hexagon h)
{
	const Orientation& M = orientation;
	double x = (M.f0 * h.GetQ() + M.f1 * h.GetR()) * size.x;
	double y = (M.f2 * h.GetQ() + M.f3 * h.GetR()) * size.y;
	return Vector2d(x + origin.x, y + origin.y);
}

Vector2d Layout::HexCornerOffset(int corner) {
	double angle = 2.0 * M_PI * (orientation.start_angle + corner) / 6;
	return Vector2d(size.x * cos(angle), size.y * sin(angle));
}

void Layout::PolygonCorners(Hexagon h, Vector2d corners[6]) {
	Vector2d center = HexToPixel(h);
	for (int i = 0; i < 6; i++) {
		Vector2d offset = HexCornerOffset(i);
		corners[i] = Vector2d{center.x + offset.x, center.y + offset.y};
	}
}

Hexagon Layout::PixelToHex(Vector2d point)
{
	Vector2d pt = Vector2d((point.x - origin.x) / size.x, (point.y - origin.y) / size.y);

	double qFrac{orientation.b0 * pt.x + orientation.b1 * pt.y};
	double rFrac{orientation.b2 * pt.x + orientation.b3 * pt.y};
	double sFrac{-rFrac-qFrac};

	int q = round(qFrac);
	int r = round(rFrac);
	int s = round(sFrac);

	double q_diff = abs(q - qFrac);
	double r_diff = abs(r - rFrac);
	double s_diff = abs(s - sFrac);

	if (q_diff > r_diff && q_diff > s_diff)
		q = -r-s;
	else if (r_diff > s_diff)
		r = -q-s;

	return Hexagon{q,r};
}