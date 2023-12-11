#include "Layout.h"

Layout::Layout(Orientation orientation_, Vector2d size_, Vector2d origin_):
	orientation(orientation_), size(size_), origin(origin_)
{
}

Vector2d Layout::HexToPixel(Hexagon h) const
{
	const Orientation& M = orientation;
	const double x = (M.f0 * h.GetQ() + M.f1 * h.GetR()) * size.x;
	const double y = (M.f2 * h.GetQ() + M.f3 * h.GetR()) * size.y;
	return Vector2d(x + origin.x, y + origin.y);
}

Vector2d Layout::HexCornerOffset(int corner) const
{
	const double angle = 2.0 * M_PI * (orientation.start_angle + corner) / 6;
	return Vector2d(size.x * cos(angle), size.y * sin(angle));
}

void Layout::PolygonCorners(Hexagon h, Vector2d corners[6]) const
{
	const Vector2d center = HexToPixel(h);
	for (int i = 0; i < 6; i++) {
		const Vector2d offset = HexCornerOffset(i);
		corners[i] = Vector2d{center.x + offset.x, center.y + offset.y};
	}
}

Hexagon Layout::PixelToHex(Vector2d point) const
{
	const Vector2d pt = Vector2d((point.x - origin.x) / size.x, (point.y - origin.y) / size.y);

	const double qFrac{orientation.b0 * pt.x + orientation.b1 * pt.y};
	const double rFrac{orientation.b2 * pt.x + orientation.b3 * pt.y};
	const double sFrac{-rFrac-qFrac};

	int q = round(qFrac);
	int r = round(rFrac);
	const int s = round(sFrac);

	const double qDiff = abs(q - qFrac);
	const double rDiff = abs(r - rFrac);
	const double sDiff = abs(s - sFrac);

	if (qDiff > rDiff && qDiff > sDiff)
		q = -r-s;
	else if (rDiff > sDiff)
		r = -q-s;

	return Hexagon{q,r};
}

Vector2d Layout::GetDistanceBetweenHexPointUp() const
{
	return Vector2d{sqrtf(3) * size.x, 3.0f / 2.0f * size.y};
}