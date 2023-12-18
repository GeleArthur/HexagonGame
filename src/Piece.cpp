#include "Piece.h"

Piece::Piece()
{
}

Piece::Piece(const SideType type1, const SideType type2, const SideType type3, const SideType type4, const SideType type5, const SideType type6)
{
	sides[0] = type1;
	sides[1] = type2;
	sides[2] = type3;
	sides[3] = type4;
	sides[4] = type5;
	sides[5] = type6;
}

Piece::Piece(const std::string &texture)
{
	if(GE->TextureFromFile(texture, pieceTexture))
	{
		for (int i{0}; i < 6; ++i)
		{
			// Assuming hex_?????? format
			sides[i] = texture[4+i] == '1' ? SideType::land : SideType::sea;
		}
	}
}