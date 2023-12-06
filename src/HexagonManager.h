#pragma once
#include "Hexagon.h"
#include "Layout.h"

class HexagonManager
{
	
	int _size;
	int cols;
	int rows;
	Layout _layout;
	Hexagon** _data;

	std::vector<Texture> _stringsASS{};

public:
	HexagonManager(int size);
	~HexagonManager();
	void Start();
	void Draw();
	Hexagon& GetHex(int r, int q) const;
};

