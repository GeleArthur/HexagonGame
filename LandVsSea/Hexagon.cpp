#include "pch.h"
#include "Hexagon.h"


Hexagon::Hexagon()
{
	
};

void Hexagon::SetUp(int q, int r, float size)
{
	this->q = q;
	this->r = r;
	this->size = size;
}

void Hexagon::GetCoords(Point2f &out)
{
	out.x = (size) * (sqrtf(3.0f) * q + sqrtf(3.0f)/2.0f * r);
	out.y = (size) * (3.0f/2.0f * r);
}

void Hexagon::DrawHexagons()
{
	Point2f coord;
	GetCoords(coord);
		
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