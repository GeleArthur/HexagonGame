#pragma once
#include <map>
#include <vector>

#include "Piece.h"
#include "UISystem.h"
#include "Hexagons/Hexagon.h"
#include "Hexagons/Layout.h"
#include "GameEngine/GameEngine.h"

class HexagonManager
{
public:
	HexagonManager(int size);
	~HexagonManager();
	void Start();
	void Draw() const;
	void Update();
	void DrawUi();

	UISystem* GetUiSystem();

private:
	Layout m_layout;
	std::unordered_map<Hexagon, Piece> m_grid{};
	UISystem* m_uiSystem;
	Piece m_placeAblePieces[6];
	int m_selectPiece;
	Piece* m_hexagonTextures;
	int m_hexagonTextureCount;

	void PlaceHex(Hexagon location, int pieceIndex);
	void DrawDebugPiece(Hexagon hex, Piece piece) const;
	void PolygonDebugPieceSide(const Hexagon &hex, int direction, Vector2d polygons[3]) const;
	int GetInvertedDirection(int direction) const;
	SideType GetSide(const Hexagon &hex, int direction) const;
	void DrawPiece(const Hexagon& hex, const Piece& piece) const;
};

