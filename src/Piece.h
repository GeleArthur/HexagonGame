﻿#pragma once
#include "Hexagons/Hexagon.h"

enum SideType
{
	land,
	sea
};

struct Piece
{
	// Hexagon hexagon;
	SideType sides[6]
	{
		land, land, land,
		land, land, land
	};
};
