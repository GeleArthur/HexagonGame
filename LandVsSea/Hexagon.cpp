#include "pch.h"
#include "Hexagon.h"

Point2f Hexagon::GetCoords()
{
	return Point2f{
		(size) * (sqrtf(3.0f) * q + sqrtf(3.0f) / 2.0f * r),
		(size) * (3.0f / 2.0f * r)
	};

}

void Hexagon::DrawHexagons()
{
	const Point2f coord{GetCoords()};
		
	Point2f hex[6]{};
	for (int k{}; k < 6; ++k)
	{
		const float angleRad = M_PI / 180 * (60 * k - 30);
		hex[k] = Point2f{coord.x + size * cosf(angleRad), coord.y + size * sinf(angleRad)};
	}

	utils::SetColor(1,1,1);
	utils::FillPolygon(hex, 6);
	utils::SetColor(0,0,0);
	utils::DrawPolygon(hex, 6);
}