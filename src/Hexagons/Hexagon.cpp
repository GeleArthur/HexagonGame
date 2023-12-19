#include "Hexagon.h"

Hexagon::Hexagon(int q, int r) : m_q(q), m_r(r)
{
}

Hexagon::Hexagon(): m_q(0), m_r(0)
{
}

void Hexagon::Set(const int q, const int r)
{
	this->m_q = q;
	this->m_r = r;
}

int Hexagon::GetS() const
{
	return -m_r-m_q;
}

int Hexagon::GetLength() const
{
	return (abs(m_q) + abs(m_r) + abs(GetS()))/2;
}

int Hexagon::GetDistance(const Hexagon other) const
{
	// Might need to flip?
	return (other-*this).GetLength();
}

Hexagon Hexagon::Direction(int direction)
{
	const Hexagon directions[6]
	{
		{1, 0}, {0, 1}, {-1, 1},
		{-1, 0}, {0, -1}, {1, -1} 
	};
	return directions[direction];
}

Hexagon Hexagon::Neighbor(int direction) const
{
	return *this + Direction(direction);
}

Hexagon Hexagon::operator+(const Hexagon other) const
{
	return Hexagon{m_q + other.m_q, m_r+other.m_r};
}

Hexagon Hexagon::operator-(const Hexagon other) const
{
	return Hexagon{m_q - other.m_q, m_r-other.m_r};
}

Hexagon Hexagon::operator*(int scaler) const
{
	return Hexagon{m_q * scaler, m_r * scaler};
}

bool Hexagon::operator==(Hexagon other) const
{
	return m_q == other.m_q && m_r == other.m_r;
}

size_t std::hash<Hexagon>::operator()(const Hexagon &other) const noexcept
{
	const size_t hashQ = std::hash<int>{}(other.GetQ());
	const size_t hashR = std::hash<int>{}(other.GetR());
	return hashQ ^ (hashR << 1); // AI said it was good.
}

int Hexagon::GetQ() const
{
	return m_q;
}

int Hexagon::GetR() const
{
	return m_r;
}