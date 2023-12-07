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
	Hexagon** _data;
	std::unordered_map<Hexagon, Piece> _newWorld{};

	std::vector<Texture> _stringsASS{};

public:
	HexagonManager(int size);
	~HexagonManager();
	void Start();
	void Draw();
	Hexagon& GetHex(int r, int q) const;
	
};

