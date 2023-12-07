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
	_grid.erase(Hexagon{0, 0});
	
	for (int repeater{1}; repeater < 5; ++repeater)
	{
		int scale = repeater;
		Hexagon currentHex{Hexagon{0,0} + Hexagon::Direction(4) * scale};
	
		for (int direction{0}; direction < 6; ++direction)
		{
			for (int k{0}; k < scale; ++k)
			{
				_grid[currentHex.Neighbor(direction)];
				currentHex = currentHex.Neighbor(direction);
			}
		}
	}
}

Piece HexagonManager::GetPiece(const Hexagon hex) const
{
	return _grid.at(hex);
}

void HexagonManager::Draw()
{
	for (const auto &piece : _grid)
	{
		Vector2d hexPolygons[6];
		_layout.PolygonCorners(piece.first, hexPolygons);
		
		GE->SetColor(0, 0, 0);
		GE->FillPolygon(hexPolygons, 6);
		GE->SetColor(1, 1, 1);
		GE->DrawPolygon(hexPolygons, 6);
	}
}

