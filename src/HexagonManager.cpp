#include "HexagonManager.h"

#include <iostream>
#include <sstream>

#include "Game.h"

HexagonManager::HexagonManager(const int size) :
	m_layout(Orientation(
		        sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
		        sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
		        /*30.0 * M_PI/180.0*/ -0.5
	        ),
	        Vector2d{static_cast<float>(size), static_cast<float>(size)},
	        Vector2d{0, 0}),
	m_selectPiece{-1}

{
	m_uiSystem = new UISystem(m_placeAblePieces, 6, m_layout, &m_selectPiece);
}

HexagonManager::~HexagonManager()
{
	delete m_uiSystem;
	delete[] m_hexagonTextures;
};

void HexagonManager::Start()
{
	// Want to do automatic file loading but c++ 14 :(
	m_hexagonTextureCount = 6;
	m_hexagonTextures = new Piece[6]
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
		m_placeAblePieces[i] = m_hexagonTextures[GE->Random(0, m_hexagonTextureCount)];
	}
	
	m_grid[Hexagon{0, 0}] = m_hexagonTextures[GE->Random(0, m_hexagonTextureCount)];
	m_grid[Hexagon{0, 0}].rotation = 0;
}

void HexagonManager::DrawDebugPiece(Hexagon hex, Piece piece) const
{
	for (int corner{0}; corner < 6; ++corner)
	{
		Vector2d polygons[3];
		PolygonDebugPieceSide(hex, corner, polygons);

		const SideType side = piece.sides[((corner - piece.rotation)+6) % 6];
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
	Vector2d center = m_layout.HexToPixel(hex);

	polygons[0] = center + m_layout.HexCornerOffset(direction);
	polygons[1] = center;
	polygons[2] = center + m_layout.HexCornerOffset(direction + 1);
}

int HexagonManager::GetInvertedDirection(int direction) const
{
	return (direction + 3) % 6;
}

SideType HexagonManager::GetSide(const Hexagon &hex, int direction) const
{
	if (m_grid.count(hex))
	{
		return m_grid.at(hex).sides[direction];
	}

	return SideType::none;
}

void HexagonManager::DrawPiece(const Hexagon &hex, const Piece &piece) const
{
	const Vector2d position = m_layout.HexToPixel(hex);
	const Vector2d size = m_layout.GetDistanceBetweenHexPointUp();
	
	Matrix4x4 rotatingMatrix{Matrix4x4::IdentityMatrix()};
	rotatingMatrix = rotatingMatrix * Matrix4x4::RotationMatrix(piece.rotation * 60 * M_PI / 180);
	rotatingMatrix = rotatingMatrix * Matrix4x4::TranslationMatrix(Vector2d{position.x /*- size.x / 2*/, position.y /*- _layout.size.y*/});
	rotatingMatrix = rotatingMatrix * GE->GetCameraMatrix(); // THIS IS COOL AS FACK HOLY SHIT

	GLfloat matrix[16];
	rotatingMatrix.OpenGlArray(matrix);
	glLoadMatrixf(matrix);
	
	GE->DrawTexture(piece.pieceTexture, Rect{-size.x/2, -m_layout.size.y, size.x, m_layout.size.y * 2}, Rect{0, 0, 0, 0});
	// GE->FillEllipse(Vector2d(0,0), 10, 10);

	
	
	GE->ApplyCamera();
}

UISystem* HexagonManager::GetUiSystem()
{
	return m_uiSystem;
}


void HexagonManager::Draw() const
{
	for (const auto &piece : m_grid)
	{
		DrawPiece(piece.first, piece.second);
		// DrawDebugPiece(piece.first, piece.second);
	}

	if (m_selectPiece != -1)
	{
		const Hexagon hex = m_layout.PixelToHex(GE->GetCameraPosition() + GE->GetMouse().position);

		DrawPiece(hex, m_placeAblePieces[m_selectPiece]);
		// DrawDebugPiece(hex, _placeAblePieces[_selectPiece]);

		Vector2d outline[6];
		m_layout.PolygonCorners(hex, outline);
		GE->SetColor(1, 1, 1);
		GE->DrawPolygon(outline, 6);
	}
}

void HexagonManager::Update()
{
	m_uiSystem->InputCheck();

	if (!m_uiSystem->IsOverUi())
	{
		if (m_selectPiece != -1)
		{
			// Is this the right way???
			if (Game::GetGame()->GetCamera()->HasClicked())
			{
				const Hexagon hex = m_layout.PixelToHex(GE->GetCameraPosition() + GE->GetMouse().position);
				if (!m_grid.count(hex))
				{
					bool isConnected{false};
					bool allowedToPlace{true};
					for (int dir{0}; dir < 6; ++dir)
					{
						Hexagon neighbor = hex.Neighbor(dir);
						if (m_grid.count(neighbor))
						{
							isConnected = true;

							const int invertDir = GetInvertedDirection(dir);
							
							if (m_placeAblePieces[m_selectPiece].sides[((dir-m_placeAblePieces[m_selectPiece].rotation)+6)%6] != GetSide(neighbor, ((invertDir - m_grid.at(neighbor).rotation)+6)%6))
							{
								allowedToPlace = false;
								break;
							}
						}
					}

					if (isConnected && allowedToPlace)
					{
						PlaceHex(hex, m_selectPiece);
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
	m_uiSystem->DrawUi();
}

void HexagonManager::PlaceHex(Hexagon location, int pieceIndex)
{
	if (!m_grid.count(location))
	{
		m_grid[location] = m_placeAblePieces[pieceIndex];
		m_placeAblePieces[pieceIndex] = m_hexagonTextures[GE->Random(0, m_hexagonTextureCount)];
		m_selectPiece = -1;
	}
}