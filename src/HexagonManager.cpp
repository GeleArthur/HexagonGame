#include "HexagonManager.h"

#include <sstream>

HexagonManager::HexagonManager(const int size) :
	_layout(Orientation(
		        sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
		        sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
		        /*30.0 * M_PI/180.0*/ -0.5
	        ),
	        Vector2d{static_cast<float>(size), static_cast<float>(size)},
	        Vector2d{0, 0}),
	_uiSystem(_placeAblePieces, 6, _layout)
{
	for (int i{0}; i < 6; ++i)
	{
		_placeAblePieces[i] = Piece{
			GE->Random(0.0f, 1.0f) > 0.5f ? SideType::sea : SideType::land,
			GE->Random(0.0f, 1.0f) > 0.5f ? SideType::sea : SideType::land,
			GE->Random(0.0f, 1.0f) > 0.5f ? SideType::sea : SideType::land,
			GE->Random(0.0f, 1.0f) > 0.5f ? SideType::sea : SideType::land,
			GE->Random(0.0f, 1.0f) > 0.5f ? SideType::sea : SideType::land,
			GE->Random(0.0f, 1.0f) > 0.5f ? SideType::sea : SideType::land
		};
	}
}

HexagonManager::~HexagonManager()
{
};

void HexagonManager::Start()
{
	_grid[Hexagon{0, 0}];
	
	for (int repeater{1}; repeater <5; ++repeater)
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

void HexagonManager::DrawDebugPiece(Hexagon hex, Piece piece) const
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
		else if (side == SideType::sea)
			GE->SetColor(0.0901, 00.3921, 0.6941);
		else if (side == SideType::none)
			continue;

		GE->FillPolygon(polygons, 3);
	}
}

SideType HexagonManager::GetNeighborSide(const Hexagon &hex, int direction) const
{
	direction = (direction+3)%6;
	const Hexagon neighbor = hex.Neighbor(direction);
	if(_grid.count(neighbor))
	{
		return _grid.at(neighbor).sides[direction];
	}

	return SideType::none;
}

void HexagonManager::Draw()
{
	for (const auto &piece : _grid)
	{
		DrawDebugPiece(piece.first, piece.second);
	}

	if(_uiSystem.GetSelectedPiece() != -1)
	{
		const Hexagon hex = _layout.PixelToHex(GE->GetCameraPosition() + GE->GetMouse().position);

		// Piece& piece = _placeAblePieces[_uiSystem.GetSelectedPiece()];
		// DrawDebugPiece(hex, piece);

		Vector2d outline[6];
		_layout.PolygonCorners(hex, outline);

		SideType side = GetNeighborSide(hex, 0);
		if(side == SideType::sea) GE->SetColor(0,0,1);
		if(side == SideType::land) GE->SetColor(0,1,1);
		GE->FillPolygon(outline, 6);
		
		// Hexagon thingIamLookingAt = hex.Neighbor(0);
		
		// Vector2d thing[6];
		// _layout.PolygonCorners(thingIamLookingAt, thing);
		// GE->FillPolygon(thing, 6);

		GE->SetColor(1,1,1);
		GE->DrawPolygon(outline, 6);
	}
}

void HexagonManager::Update()
{
	_uiSystem.InputCheck();
}

void HexagonManager::DrawUi()
{
	_uiSystem.DrawUi();
}

