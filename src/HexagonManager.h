#pragma once
#include <map>
#include <vector>

#include "Piece.h"
#include "Hexagons/Hexagon.h"
#include "Hexagons/Layout.h"
#include "GameEngine/GameEngine.h"

class HexagonManager
{
	int _size;
	int cols;
	int rows;
	Layout _layout;
	std::unordered_map<Hexagon, Piece> _grid{};
	std::vector<Texture> _stringsASS{};

public:
	HexagonManager(int size);
	~HexagonManager();
	void Start();
	void Draw();
	Piece GetPiece(Hexagon hex) const;
	void DrawDebugPiece(Hexagon hex, Piece piece);
};

