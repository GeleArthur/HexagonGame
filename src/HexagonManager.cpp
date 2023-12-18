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
	        Vector2d{0, 0}),
	_selectPiece{-1}

{
	_uiSystem = new UISystem(_placeAblePieces, 6, _layout, &_selectPiece);
}

HexagonManager::~HexagonManager()
{
};

void HexagonManager::Start()
{
	// Want to do automatic file loading but c++ 14 :(
	_hexagonTextureCount = 6;
	_hexagonTextures = new Piece[6]
	{
		Piece{"hex_122211.png"},
		Piece{"hex_122211_1.png"},
		Piece{"hex_211122.png"},
		Piece{"hex_211222.png"},
		Piece{"hex_212211.png"},
		Piece{"hex_221211.png"},
	};

	for (int i{0}; i < 6; ++i)
	{
		_placeAblePieces[i] = _hexagonTextures[GE->Random(0, _hexagonTextureCount)];
	}
	
	_grid[Hexagon{0, 0}] = _hexagonTextures[GE->Random(0, _hexagonTextureCount)];
	_grid[Hexagon{0, 0}].rotation = 0;
	// for (int repeater{1}; repeater <5; ++repeater)
	// {
	// 	const int scale = repeater;
	// 	Hexagon currentHex{Hexagon{0,0} + Hexagon::Direction(4) * scale};
	//
	// 	for (int direction{0}; direction < 6; ++direction)
	// 	{
	// 		for (int k{0}; k < scale; ++k)
	// 		{
	// 			_grid[currentHex.Neighbor(direction)] = Piece{
	// 				GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
	// 				GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
	// 				GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
	// 				GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
	// 				GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land,
	// 				GE->Random(0.0f,1.0f) > 0.5f ? SideType::sea: SideType::land
	// 			};
	//
	// 			currentHex = currentHex.Neighbor(direction);
	// 		}
	// 	}
	// }


	for (auto &hexagon : _grid)
	{
		for (int dir{0}; dir < 6; ++dir)
		{
			Hexagon neighbor = hexagon.first.Neighbor(dir);
			if (_grid.count(neighbor))
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
		if (side == SideType::land)
			GE->SetColor(0.6941, 0.4745, 0.0901, 0.5);
		else if (side == SideType::sea)
			GE->SetColor(0.0901, 00.3921, 0.6941, 0.5);
		else if (side == SideType::none) continue;

		GE->FillPolygon(polygons, 3);
	}
}

void HexagonManager::PolygonDebugPieceSide(const Hexagon &hex, int direction, Vector2d polygons[3]) const
{
	Vector2d center = _layout.HexToPixel(hex);

	polygons[0] = center + _layout.HexCornerOffset(direction);
	polygons[1] = center;
	polygons[2] = center + _layout.HexCornerOffset(direction + 1);
}

int HexagonManager::GetInvertedDirection(int direction) const
{
	return (direction + 3) % 6;
}

SideType HexagonManager::GetSide(const Hexagon &hex, int direction) const
{
	if (_grid.count(hex))
	{
		return _grid.at(hex).sides[direction];
	}

	return SideType::none;
}

void HexagonManager::DrawPiece(const Hexagon &hex, const Piece &piece) const
{
	const Vector2d position = _layout.HexToPixel(hex);
	const Vector2d size = _layout.GetDistanceBetweenHexPointUp();
	
	Matrix4x4 rotatingMatrix{Matrix4x4::IdenityMatrix()};
	rotatingMatrix = rotatingMatrix * Matrix4x4::RotationMatrix(piece.rotation * 30 * M_PI / 180);
	rotatingMatrix = rotatingMatrix * GE->GetCameraMatrix(); // THIS IS COOL AS FACK HOLY SHIT

	GLfloat matrix[16];
	rotatingMatrix.openGlArray(matrix);
	glLoadMatrixf(matrix);

	GE->DrawTexture(piece.pieceTexture, Rect{position.x - size.x / 2, position.y - _layout.size.y, size.x, _layout.size.y * 2}, Rect{0, 0, 0, 0});
	GE->FillEllipse(position, 10, 10);

	GE->ApplyCamera();
}

UISystem* HexagonManager::GetUiSystem()
{
	return _uiSystem;
}


void HexagonManager::Draw() const
{
	for (const auto &piece : _grid)
	{
		DrawPiece(piece.first, piece.second);
		DrawDebugPiece(piece.first, piece.second);
	}

	if (_selectPiece != -1)
	{
		const Hexagon hex = _layout.PixelToHex(GE->GetCameraPosition() + GE->GetMouse().position);

		DrawPiece(hex, _placeAblePieces[_selectPiece]);

		Vector2d outline[6];
		_layout.PolygonCorners(hex, outline);
		GE->SetColor(1, 1, 1);
		GE->DrawPolygon(outline, 6);
	}
}

void HexagonManager::Update()
{
	_uiSystem->InputCheck();

	if (!_uiSystem->IsOverUi())
	{
		if (_selectPiece != -1)
		{
			// Is this the right way???
			if (Game::GetGamePtr()->GetCamera()->HasClicked())
			{
				const Hexagon hex = _layout.PixelToHex(GE->GetCameraPosition() + GE->GetMouse().position);
				
				
				if (!_grid.count(hex))
				{
					bool isConnected{false};
					bool allowedToPlace{true};
					for (int dir{0}; dir < 6; ++dir)
					{
						Hexagon neighbor = hex.Neighbor(dir);
						if (_grid.count(neighbor))
						{
							isConnected = true;

							const int invertDir = GetInvertedDirection(dir);
							if (_placeAblePieces[_selectPiece].sides[dir] != GetSide(neighbor, invertDir))
							{
								allowedToPlace = false;
								break;
							}
						}
					}

					if (isConnected && allowedToPlace)
					{
						PlaceHex(hex, _selectPiece);
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
	if (!_grid.count(location))
	{
		_grid[location] = _placeAblePieces[pieceIndex];
		_placeAblePieces[pieceIndex] = _hexagonTextures[GE->Random(0, _hexagonTextureCount)];
		_selectPiece = -1;
	}
}