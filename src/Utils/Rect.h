#pragma once

struct Rect
{
	Rect( );
	explicit Rect( float left, float top, float width, float height );
	
	float left;
	float top;
	float width;
	float height;
};
