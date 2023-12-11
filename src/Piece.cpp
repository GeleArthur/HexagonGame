#include "Piece.h"

Piece::Piece()
{
}

Piece::Piece(SideType type1, SideType type2, SideType type3, SideType type4, SideType type5, SideType type6)
{
	sides[0] = type1;
	sides[1] = type2;
	sides[2] = type3;
	sides[3] = type4;
	sides[4] = type5;
	sides[5] = type6;
}