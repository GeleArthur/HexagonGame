#pragma once
#include "GameEngine/GameEngine.h"
#include "Utils/Mouse.h"

class CameraSystem
{
private:
	Vector2d m_mousePrevFrame;
	Rect m_boundingBox;
	bool m_hasClicked;
	
public:
	CameraSystem(const Rect& newBoundingBox);
	void Start();
	void UpdateCamera();
	void DrawBoundingBox() const;
	bool HasClicked() const;
};
