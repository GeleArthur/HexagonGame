#include "HexagonManager.h"

#include <sstream>

HexagonManager::HexagonManager(int size) : _size(size),
                                           _layout(Orientation(sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0,
                                                               sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0,
                                                               0.5), Vector2d{float(size), float(size)}, Vector2d{0, 0})
{
	rows = static_cast<int>(floor(GE->GetWindowWidth() / size));
	cols = static_cast<int>(floor(GE->GetWindowHeight()*3.0/2.0 / size));
	rows = cols;

	_data = new Hexagon *[rows];
	for (int row{0}; row < rows; ++row)
	{
		_data[row] = new Hexagon[cols];

		//  Kinda dum that I have to set it twice. Is there a better way :thinking:
		for (int col{0}; col < cols; ++col)
		{
			_data[row][col].Set(
				col - floor(row / 2),
				row
				);

			// _data[row][col].size = 10;
		}
	}


};

void HexagonManager::Start()
{
	for (int i{0}; i < rows; ++i)
	{
		for (int j{0}; j < cols; ++j)
		{
			std::stringstream buffer;
			
			buffer << "q: " << _data[i][j].GetQ() << " r: " << _data[i][j].GetR();
			Texture text;
			GE->TextureFromString(buffer.str(), "C:\\arial.ttf", 16, text);
			
			_stringsASS.push_back(text);
		}
	}
}

HexagonManager::~HexagonManager()
{
	for (int i = 0; i < rows; ++i)
	{
		delete[] _data[i];
	}
	delete[] _data;
}



Hexagon& HexagonManager::GetHex(int r, int q) const
{
	return _data[r][static_cast<int>(q - floor(r / 2))];
}

void HexagonManager::Draw()
{
	// auto mouse = GE->GetMouse();
	// _layout.origin = Vector2d{mouse.x, mouse.y};
	
	for (int x{0}; x < cols; ++x)
	{
		for (int y{0}; y < rows; ++y)
		{
			Vector2d hex[6];
			_layout.polygon_corners(_data[x][y], hex);
			
			GE->SetColor(0,0,0);
			GE->FillPolygon(hex, 6);
			GE->SetColor(1,1,1);
			GE->DrawPolygon(hex, 6);

			Vector2d size{_stringsASS[x*rows + y].width, _stringsASS[x*rows + y].height};

			GE->DrawTexture(_stringsASS[x*rows + y], _layout.hex_to_pixel(_data[x][y])-size/2);
		}
	}
}

