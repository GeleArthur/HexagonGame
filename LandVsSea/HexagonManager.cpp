#include "pch.h"
#include "HexagonManager.h"

void HexagonManager::SetData(const int width, const int height, const float size)
{
	this->width = width;
	this->height = height;
	data = new Hexagon *[width];

	for (int i = 0; i < width; ++i)
	{
		data[i] = new Hexagon[height];
	}

	for (int x{0}; x < width; ++x)
	{
		for (int y{0}; y < height; ++y)
		{
			data[x][y].SetUp(x,y,size);
		}
	}
}

HexagonManager::HexagonManager(): data(nullptr), width(0), height(0)
{};

HexagonManager::~HexagonManager()
{
	for (int i = 0; i < width; ++i)
	{
		delete data[i];
	}
	delete[] data;
}
