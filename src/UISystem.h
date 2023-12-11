#pragma once
// #include "HexagonManager.h"
#include "Piece.h"
#include "Hexagons/Layout.h"

class UISystem
{
	Layout _layout;
	Piece* _selectAblePieces;
	int _piecesCount{};
	int _selectedPieceIndex{-1};
	
public:
	UISystem(Piece* selectAblePieces, int pieceCount, const Layout& layoutCopy);
	void DrawUi();
	void InputCheck();
	void DrawDebugPiece(Vector2d position, const Piece& piece) const;
	Vector2d GetHexPosition(int index) const;
	void DrawOutLineSelected() const;
	int GetSelectedPiece() const;
	
};
