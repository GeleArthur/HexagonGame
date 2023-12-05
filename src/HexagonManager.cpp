#include "HexagonManager.h"

HexagonManager::HexagonManager(int size) : _size(size),
                                           _layout(Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
                                                               sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
                                                               0.5), Vector2d{10, 10}, Vector2d{0, 0})
{
	int maxRows{static_cast<int>(floor(GE->GetWindowWidth() / size))};
	int maxCols{static_cast<int>(floor(GE->GetWindowHeight() / size))};
	rows = maxRows;
	cols = maxCols;

	_data = new Hexagon *[maxRows];
	for (int row{0}; row < maxRows; ++row)
	{
		_data[row] = new Hexagon[maxCols];

		//  Kinda dum that I have to set it twice. Is there a better way :thinking:
		for (int col{0}; col < maxCols; ++col)
		{
			_data[row][col].q = (col - floor(row / 2)) - maxCols/2;
			_data[row][col].r = (row) - maxRows/2;
			// _data[row][col].size = 10;
		}
	}
};

HexagonManager::~HexagonManager()
{
	for (int i = 0; i < rows; ++i)
	{
		delete[] _data[i];
	}
	delete[] _data;
}

Hexagon& HexagonManager::GetHex(int r, int q)
{
	return _data[r][static_cast<int>(q - floor(r / 2))];
}

void HexagonManager::Draw()
{
	auto mouse = GE->GetMouse();
	_layout.origin = Vector2d{mouse.x, mouse.y};
	
	for (int x{0}; x < cols; ++x)
	{
		for (int y{0}; y < rows; ++y)
		{
			Vector2d hex[6];
			_layout.polygon_corners(_data[x][y], hex);
			
			GE->SetColor(1,1,1);
			GE->FillPolygon(hex, 6);
			GE->SetColor(0,0,0);
			GE->DrawPolygon(hex, 6);
		}
	}
}

