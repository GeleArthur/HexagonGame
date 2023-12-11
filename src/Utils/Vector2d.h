#pragma once

struct Vector2d
{
	Vector2d();
	explicit Vector2d(float x, float y);

	float x;
	float y;

    void operator +=(const Vector2d &vector);
    void operator -=(const Vector2d &vector);
    bool operator !=(const Vector2d &vector);
	Vector2d& operator /=(float scaler);
	Vector2d& operator *=(float scaler);
	Vector2d operator +(Vector2d v2);
	Vector2d operator -(Vector2d v2);
	Vector2d operator *(Vector2d v2);
	Vector2d operator /(Vector2d v2);
	Vector2d operator *(float scaler);
	Vector2d operator /(float scaler);
};