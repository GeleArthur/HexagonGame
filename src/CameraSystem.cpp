#include "CameraSystem.h"

#include "Game.h"

CameraSystem::CameraSystem(const Rect& newBoundingBox): _mousePrevFrame(0,0), _boundingBox(newBoundingBox)
{
}

void CameraSystem::Start()
{
	GE->SetCameraPosition(Vector2d{-GE->GetWindowWidth()/2.0f, -GE->GetWindowHeight()/2.0f});
	GE->ApplyCamera();
}

void CameraSystem::UpdateCamera()
{
	if(!Game::GetGame()->GetHexagonManager()->GetUiSystem()->IsOverUi())
	{
		Mouse currentMouse{GE->GetMouse()};
		if(currentMouse.left.downThisFrame)
		{
			_hasClicked = true;
			_mousePrevFrame = currentMouse.position;
		}
		if(currentMouse.left.holdingDown)
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
				_hasClicked = false;
				GE->SetCameraPosition(realCameraPos);
				GE->ApplyCamera();
			}
		
			_mousePrevFrame = currentMouse.position;
		}
	}
}

void CameraSystem::DrawBoundingBox() const
{
	GE->SetColor(0, 1, 0);
	GE->DrawRect(_boundingBox.left, _boundingBox.top, _boundingBox.width, _boundingBox.height);
}

bool CameraSystem::HasClicked() const
{
	if(GE->GetMouse().left.upThisFrame)
	{
		return _hasClicked;
	}
	
	return false;
}