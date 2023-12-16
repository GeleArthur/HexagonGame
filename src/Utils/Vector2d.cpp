#include "Vector2d.h"

#include <corecrt_math.h>

Vector2d::Vector2d() :Vector2d{ 0.0f, 0.0f }{}

Vector2d::Vector2d(float x, float y) :x{ x }, y{ y }{}

//Vector2d& Vector2d::operator+=(const Vector2d *vector)
//{
//	x += vector.x;
//	y += vector.y;
//	return *this;
//}

void Vector2d::operator+=(const Vector2d &vector) {
	x += vector.x;
	y += vector.y;
}

void Vector2d::operator-=(const Vector2d &vector)
{
	x -= vector.x;
	y -= vector.y;
}

bool Vector2d::operator!=(const Vector2d &vector) const
{
	return x != vector.x || y != vector.y;
}

bool Vector2d::operator==(const Vector2d &vector) const
{
	return x == vector.x || y == vector.y;
}

Vector2d & Vector2d::operator/=(float scaler)
{
	x /= scaler;
	y /= scaler;
	return *this;
}

Vector2d & Vector2d::operator*=(float scaler)
{
	x *= scaler;
	y *= scaler;
	return *this;
}

Vector2d Vector2d::operator+(Vector2d v2) const
{
	return Vector2d(x+v2.x, y+v2.y);
}

Vector2d Vector2d::operator-(Vector2d v2) const
{
	return Vector2d(x-v2.x, y-v2.y);
}

Vector2d Vector2d::operator*(Vector2d v2) const
{
	return Vector2d(x*v2.x, y*v2.y);
}

Vector2d Vector2d::operator/(Vector2d v2) const
{
	return Vector2d(x/v2.x, y/v2.y);
}

Vector2d Vector2d::operator*(float scaler) const
{
	return Vector2d(x*scaler, y*scaler);
}

Vector2d Vector2d::operator/(float scaler) const
{
	return Vector2d(x/scaler, y/scaler);
}

float Vector2d::Length() const
{
	return sqrtf(x*x+y*y);
}


