#include "HexagonManager.h"

#include <sstream>

HexagonManager::HexagonManager(int size) :
	_size(size),
	_layout(Orientation(
				sqrt(3.0)       , sqrt(3.0) / 2.0 , 0.0, 3.0 / 2.0,
			    sqrt(3.0) / 3.0 , -1.0 / 3.0      , 0.0, 2.0 / 3.0,
			    0.5
			),	
		    Vector2d{static_cast<float>(size), static_cast<float>(size)},
		    Vector2d{0, 0})
{
}

HexagonManager::~HexagonManager()
{
};

void HexagonManager::Start()
{
	_grid[Hexagon{0, 0}];
	// _grid.erase(Hexagon{0, 0});
	
	for (int repeater{1}; repeater < 5; ++repeater)
	{
		int scale = repeater;
		Hexagon currentHex{Hexagon{0,0} + Hexagon::Direction(4) * scale};
	
		for (int direction{0}; direction < 6; ++direction)
		{
			for (int k{0}; k < scale; ++k)
			{
				_grid[currentHex.Neighbor(direction)] = Piece{
					GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
					GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
					GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
					GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
					GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
					GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land
				};
				currentHex = currentHex.Neighbor(direction);
			}
		}
	}
}

Piece HexagonManager::GetPiece(const Hexagon hex) const
{
	return _grid.at(hex);
}

void HexagonManager::DrawDebugPiece(Hexagon hex, Piece piece)
{
	Vector2d center = _layout.HexToPixel(hex);

	for (int corner{0}; corner < 6; ++corner)
	{
		Vector2d polygons[3];
		polygons[0] = center + _layout.HexCornerOffset(corner);
		polygons[1] = center;
		polygons[2] = center + _layout.HexCornerOffset(corner+1);

		const SideType side = piece.sides[corner];
		if(side == SideType::land)
			GE->SetColor(0.6941, 0.4745, 0.0901);
		else
			GE->SetColor(0.0901, 00.3921, 0.6941);

		GE->FillPolygon(polygons, 3);
	}
}

void HexagonManager::Draw()
{
	for (const auto &piece : _grid)
	{
		DrawDebugPiece(piece.first, piece.second);
	}

	const Hexagon hex = _layout.PixelToHex(GE->GetCameraPosition() + GE->GetMouse().position);

	Vector2d hexPolygons[6];
	_layout.PolygonCorners(hex, hexPolygons);
	GE->SetColor(1, 0, 0);
	GE->FillPolygon(hexPolygons, 6);
}

