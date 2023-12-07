#include "Hexagon.h"

Hexagon::Hexagon(int q, int r) : q(q), r(r)
{
}

Hexagon::Hexagon(): q(0), r(0)
{
}

void Hexagon::Set(const int q, const int r)
{
	this->q = q;
	this->r = r;
}

int Hexagon::GetS() const
{
	return -r-q;
}

int Hexagon::GetLength() const
{
	return (abs(q) + abs(r) + abs(GetS()))/2;
}

int Hexagon::GetDistance(const Hexagon other) const
{
	// Might need to flip?
	return (other-*this).GetLength();
}

Hexagon Hexagon::Direction(int direction)
{
	constexpr Hexagon directions[6]
	{
		{1, 0}, {1, -1}, {0, -1},
		{-1, 0}, {-1, 1}, {0, 1}
	};
	return directions[direction];
}

Hexagon Hexagon::Neighbor(int direction) const
{
	return *this + Direction(direction);
}

Hexagon Hexagon::operator+(const Hexagon other) const
{
	return Hexagon{q + other.q, r+other.r};
}

Hexagon Hexagon::operator-(const Hexagon other) const
{
	return Hexagon{q - other.q, r-other.r};
}

Hexagon Hexagon::operator*(int scaler) const
{
	return Hexagon{q * scaler, r * scaler};
}

bool Hexagon::operator==(Hexagon other) const
{
	return q == other.q && r == other.r;
}

int Hexagon::GetQ() const
{
	return q;
}

int Hexagon::GetR() const
{
	return r;
}