﻿#pragma once
#include "Hexagons/Hexagon.h"

enum class SideType
{
	land,
	sea
};

struct Piece
{
	Piece();
	Piece(SideType type1, SideType type2, SideType type3, SideType type4, SideType type5, SideType type6);
	
	SideType sides[6]
	{
		SideType::land, SideType::land, SideType::land,
		SideType::land, SideType::land, SideType::land
	};
};