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
	void Draw();
	void Update();
	void DrawUi();
	Piece GetPiece(Hexagon hex) const;
	void DrawDebugPiece(Hexagon hex, Piece piece) const;
	SideType GetNeighborSide(const Hexagon& hex, int direction) const;
	
private:
	Layout _layout;
	std::unordered_map<Hexagon, Piece> _grid{};
	UISystem _uiSystem;
	Piece _placeAblePieces[6];
};

