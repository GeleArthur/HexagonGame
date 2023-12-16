#pragma once
#include "GameEngine/GameEngine.h"
#include "Utils/Mouse.h"

class CameraSystem
{
private:
	Vector2d _mousePrevFrame;
	Rect _boundingBox;
	bool _hasClicked;
	
public:
	CameraSystem(const Rect& newBoundingBox);
	void Start();
	void UpdateCamera();
	void DrawBoundingBox() const;
	bool HasClicked() const;
};
