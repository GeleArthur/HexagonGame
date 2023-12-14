#pragma once
#include "GameEngine/GameEngine.h"
#include "Utils/Mouse.h"

class CameraSystem
{
	Vector2d _mousePrevFrame;
	Rect _boundingBox;
	
public:
	CameraSystem(const Rect& newBoundingBox);
	void Start();
	void UpdateCamera();
	void DrawBoundingBox();
};
