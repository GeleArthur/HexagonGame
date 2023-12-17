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
	void PlaceHex(Hexagon location, int pieceIndex);
	void DrawDebugPiece(Hexagon hex, Piece piece) const;
	void PolygonDebugPieceSide(const Hexagon &hex, int direction, Vector2d polygons[3]) const;
	int GetInvertedDirection(int direction) const;
	SideType GetSide(const Hexagon &hex, int direction) const;
	UISystem* GetUiSystem();

private:
	Layout _layout;
	std::unordered_map<Hexagon, Piece> _grid{};
	UISystem* _uiSystem;
	Piece _placeAblePieces[6];
	int _selectPiece;
};

