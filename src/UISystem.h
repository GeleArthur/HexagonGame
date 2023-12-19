#pragma once
// #include "HexagonManager.h"
#include "Piece.h"
#include "Hexagons/Layout.h"

class UISystem
{
	Layout m_layout;
	Piece* m_selectAblePieces;
	int m_piecesCount{};
	int* m_selectedPieceIndex;
	
public:
	UISystem(Piece* selectAblePieces, int pieceCount, const Layout& layoutCopy, int* selectedPieceIndex);
	void DrawUi() const;
	void InputCheck() const;
	void DrawDebugPiece(Vector2d position, const Piece& piece) const;
	void DrawPiece(Vector2d position, const Piece &piece) const;
	Vector2d GetHexPosition(int index) const;
	void DrawOutLineSelected() const;
	bool IsOverUi() const;
};
