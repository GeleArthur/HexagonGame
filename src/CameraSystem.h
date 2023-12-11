#pragma once
#include "GameEngine/GameEngine.h"
#include "Utils/Mouse.h"

class CameraSystem
{
	Vector2d _mousePrevFrame;
	Rect _boundingBox;
	
public:
	CameraSystem();
	void Start();
	void UpdateCamera();
	void SetBoundingBox(const Rect& newBoundingBox);
	void DrawBoundingBox();
};
