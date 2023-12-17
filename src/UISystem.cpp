#include "UISystem.h"

UISystem::UISystem(Piece *selectAblePieces, const int pieceCount, const Layout& layoutCopy, int* selectedPieceIndex):
	_layout(layoutCopy),
	_selectAblePieces(selectAblePieces),
	_piecesCount(pieceCount),
	_selectedPieceIndex(selectedPieceIndex)
{
	_layout.size *= 1.3;
}

void UISystem::DrawUi() const
{
	const Vector2d size{_layout.GetDistanceBetweenHexPointUp()};

	
	GE->SetColor(77.0f/255.0f, 44.0f/255.0f, 13.0f/255.0f);
	GE->FillRect(0, GE->GetWindowHeight() - size.y*2, GE->GetWindowWidth(), size.y*2);
	
	for (int i{0}; i < _piecesCount; ++i)
	{
		DrawDebugPiece(GetHexPosition(i), _selectAblePieces[i]);
	}
	
	DrawOutLineSelected();
}

void UISystem::InputCheck() const
{
	Mouse mouse = GE->GetMouse();

	if(mouse.left.downThisFrame)
	{
		for (int i{0}; i < _piecesCount; ++i)
		{
			Vector2d position = GetHexPosition(i);
			float distance = (position-mouse.position).Length();
			if(distance < _layout.size.x) // Bad code
			{
				*_selectedPieceIndex = i;
				break;
			}
		}
	}

}

void UISystem::DrawDebugPiece(Vector2d position, const Piece& piece) const
{
	for (int corner{0}; corner < 6; ++corner)
	{
		Vector2d polygons[3];
		polygons[0] = position + _layout.HexCornerOffset(corner);
		polygons[1] = position;
		polygons[2] = position + _layout.HexCornerOffset(corner+1);

		const SideType side = piece.sides[corner];
		if(side == SideType::land)
			GE->SetColor(0.6941, 0.4745, 0.0901);
		else
			GE->SetColor(0.0901, 00.3921, 0.6941);

		GE->FillPolygon(polygons, 3);
	}
}

Vector2d UISystem::GetHexPosition(int index) const
{
	Vector2d size{_layout.GetDistanceBetweenHexPointUp()};
	const float moveAmount{(GE->GetWindowWidth()-size.x)/_piecesCount};
	return Vector2d{size.x*1.5f + index * moveAmount, GE->GetWindowHeight()-size.y};
}

void UISystem::DrawOutLineSelected() const
{
	if(*_selectedPieceIndex != -1)
	{
		const Vector2d position = GetHexPosition(*_selectedPieceIndex);
		Vector2d outline[6];
		for (int i = 0; i < 6; i++) {
			const Vector2d offset = _layout.HexCornerOffset(i);
			outline[i] = Vector2d{position.x + offset.x, position.y + offset.y};
		}
		GE->SetColor(1,1,1);
		GE->DrawPolygon(outline, 6, true, 4);
	}
}

bool UISystem::IsOverUi() const
{
	const Vector2d size{_layout.GetDistanceBetweenHexPointUp()};
	return GE->GetMouse().position.y > GE->GetWindowHeight() - size.y*2;
}