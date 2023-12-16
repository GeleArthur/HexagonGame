#include "HexagonManager.h"

#include <iostream>
#include <sstream>

#include "Game.h"

HexagonManager::HexagonManager(const int size) :
	_layout(Orientation(
		        sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
		        sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
		        /*30.0 * M_PI/180.0*/ -0.5
	        ),
	        Vector2d{static_cast<float>(size), static_cast<float>(size)},
	        Vector2d{0, 0})
{
	_uiSystem = new UISystem(_placeAblePieces, 6, _layout);
	
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
		const int scale = repeater;
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


	for (auto& hexagon : _grid)
	{
		for (int dir{0}; dir < 6; ++dir)
		{
			Hexagon neighbor = hexagon.first.Neighbor(dir);
			if(_grid.count(neighbor))
			{
				const int invertedDir = GetInvertedDirection(dir);
				hexagon.second.sides[dir] = GetSide(neighbor, invertedDir);
			}
		}
	}
}

void HexagonManager::DrawDebugPiece(Hexagon hex, Piece piece) const
{
	for (int corner{0}; corner < 6; ++corner)
	{
		Vector2d polygons[3];
		PolygonDebugPieceSide(hex, corner, polygons);

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

void HexagonManager::PolygonDebugPieceSide(const Hexagon& hex, int direction, Vector2d polygons[3]) const
{
	Vector2d center = _layout.HexToPixel(hex);
	
	polygons[0] = center + _layout.HexCornerOffset(direction);
	polygons[1] = center;
	polygons[2] = center + _layout.HexCornerOffset(direction+1);
}

int HexagonManager::GetInvertedDirection(int direction) const
{
	 return (direction+3)%6;
}

SideType HexagonManager::GetSide(const Hexagon &hex, int direction) const
{
	if(_grid.count(hex))
	{
		return _grid.at(hex).sides[direction];
	}

	return SideType::none;
}

UISystem * HexagonManager::GetUiSytem()
{
	return _uiSystem;
}


void HexagonManager::Draw() const
{
	for (const auto &piece : _grid)
	{
		DrawDebugPiece(piece.first, piece.second);
	}

	const int selectedPiece = _uiSystem->GetSelectedPiece();
	if(selectedPiece != -1)
	{
		const Hexagon hex = _layout.PixelToHex(GE->GetCameraPosition() + GE->GetMouse().position);
		
		DrawDebugPiece(hex, _placeAblePieces[selectedPiece]);
		
		Vector2d outline[6];
		_layout.PolygonCorners(hex, outline);
		GE->SetColor(1,1,1);
		GE->DrawPolygon(outline, 6);
	}
}

void HexagonManager::Update()
{
	_uiSystem->InputCheck();


	if(!_uiSystem->IsOverUi())
	{
		const int selectedPiece = _uiSystem->GetSelectedPiece();
		if(selectedPiece != -1)
		{
			// Is this the right way???
			if(Game::GetGamePtr()->GetCamera()->HasClicked())
			{
				const Hexagon hex = _layout.PixelToHex(GE->GetCameraPosition() + GE->GetMouse().position);

				if(!_grid.count(hex))
				{
					bool isConnected{false};
					bool allowedToPlace{true};
					for (int dir{0}; dir < 6; ++dir)
					{
						Hexagon neighbor = hex.Neighbor(dir);
						if(_grid.count(neighbor))
						{
							isConnected = true;

							const int invertDir = GetInvertedDirection(dir);
							if(_placeAblePieces[selectedPiece].sides[dir] != GetSide(neighbor, invertDir) )
							{
								allowedToPlace = false;
								break;
							}
						}
					}

					if(isConnected && allowedToPlace)
					{
						PlaceHex(hex, selectedPiece);
					}
					else
					{
						std::cout << "NO";
						// Play error animation
					}
				}
				else
				{
					std::cout << "NO";
					// Play error animation
				}

			}
		}
	}
}

void HexagonManager::DrawUi()
{
	_uiSystem->DrawUi();
}


void HexagonManager::PlaceHex(Hexagon location, int pieceIndex)
{
	if(!_grid.count(location))
	{
		_grid[location] = _placeAblePieces[pieceIndex];
		_placeAblePieces[pieceIndex] = Piece{
			GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
			GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
			GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
			GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
			GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
			GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land
		};
	}
}
