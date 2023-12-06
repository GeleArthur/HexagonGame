#include "Hexagon.h"

#include "Layout.h"

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
	// Might need to flip???
	return (other-*this).GetLength();
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

int Hexagon::GetQ() const
{
	return q;
}

int Hexagon::GetR() const
{
	return r;
}