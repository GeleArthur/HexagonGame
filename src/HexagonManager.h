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

public:
	HexagonManager(int size);
	~HexagonManager();
	void Draw();
	Hexagon& GetHex(int r, int q);
};

