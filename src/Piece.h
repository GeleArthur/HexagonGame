#pragma once
#include "Hexagons/Hexagon.h"

enum class SideType
{
	none,
	land,
	sea,
	
	count
};

struct Piece
{
	Piece();
	Piece(SideType type1, SideType type2, SideType type3, SideType type4, SideType type5, SideType type6);
	Piece(const std::string &texture);
	
	SideType sides[6]
	{
		SideType::land, SideType::land, SideType::land,
		SideType::land, SideType::land, SideType::land
	};

	int rotation{0};
	Texture pieceTexture;
};
