#include "CameraSystem.h"

CameraSystem::CameraSystem(): _mousePrevFrame(0,0), _boundingBox()
{
}

void CameraSystem::Start()
{
	GE->SetCameraPosition(Vector2d{-GE->GetWindowWidth()/2.0f, -GE->GetWindowHeight()/2.0f});
	GE->ApplyCamera();
}

void CameraSystem::UpdateCamera()
{
	Mouse currentMouse{GE->GetMouse()};
	if(currentMouse.right.downThisFrame)
	{
		_mousePrevFrame = currentMouse.position;
	}
	if(currentMouse.right.holdingDown)
	{
		Vector2d currentPosition{GE->GetCameraPosition()};
		const Vector2d newCameraPosition{currentPosition - (currentMouse.position - _mousePrevFrame)};
		Vector2d realCameraPos{currentPosition};
		
		if(newCameraPosition.x > _boundingBox.left && newCameraPosition.x < _boundingBox.left+_boundingBox.width)
		{
			realCameraPos.x = newCameraPosition.x;
		}
		if(newCameraPosition.y > _boundingBox.top && newCameraPosition.y < _boundingBox.top+_boundingBox.height)
		{
			realCameraPos.y = newCameraPosition.y;
		}
		
		if(realCameraPos != currentPosition)
		{
			GE->SetCameraPosition(realCameraPos);
			GE->ApplyCamera();
		}
		
		_mousePrevFrame = currentMouse.position;
	}
}

void CameraSystem::SetBoundingBox(const Rect &newBoundingBox)
{
	_boundingBox = newBoundingBox;
}

void CameraSystem::DrawBoundingBox()
{
	GE->SetColor(0, 1, 0);
	GE->DrawRect(_boundingBox.left, _boundingBox.top, _boundingBox.width, _boundingBox.height);
}