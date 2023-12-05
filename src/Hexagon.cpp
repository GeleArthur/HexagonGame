#include "Hexagon.h"

#include "Layout.h"

const Hexagon hex_directions[6] {
	Hexagon(1, 0), Hexagon(1, -1), Hexagon(0, -1), 
	Hexagon(-1, 0), Hexagon(-1, 1), Hexagon(0, 1)
};

Hexagon::Hexagon(int q, int r) : q(q), r(r)
{
	
}

Hexagon::Hexagon(): q(0), r(0)
{
};

Vector2d Hexagon::GetCoords()
{
	return Vector2d{
		/*(size) **/ (sqrtf(3.0f) * q + sqrtf(3.0f) / 2.0f * r),
		/*(size) **/ (3.0f / 2.0f * r)
	};
}

void Hexagon::DrawHexagons()
{
	float sizeX{sqrtf(3)*10};
	float sizeY{3.0f/4*10};
	
	const Vector2d coords{ GetCoords() };
	
	Vector2d hex[6]{};
	for (int k{}; k < 6; ++k)
	{
		const float angleRad = M_PI / 180 * (60 * k - 30);
		hex[k] = Vector2d{sizeX*coords.x + sizeX * cosf(angleRad), sizeY* coords.y + sizeY * sinf(angleRad)};
	}
	

}

int Hexagon::GetS()
{
	return -r-q;
}