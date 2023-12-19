#include "CameraSystem.h"

#include "Game.h"

CameraSystem::CameraSystem(const Rect& newBoundingBox): m_mousePrevFrame(0,0), m_boundingBox(newBoundingBox)
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
			m_hasClicked = true;
			m_mousePrevFrame = currentMouse.position;
		}
		if(currentMouse.left.holdingDown)
		{
			Vector2d currentPosition{GE->GetCameraPosition()};
			const Vector2d newCameraPosition{currentPosition - (currentMouse.position - m_mousePrevFrame)};
			Vector2d realCameraPos{currentPosition};
		
			if(newCameraPosition.x > m_boundingBox.left && newCameraPosition.x < m_boundingBox.left+m_boundingBox.width)
			{
				realCameraPos.x = newCameraPosition.x;
			}
			if(newCameraPosition.y > m_boundingBox.top && newCameraPosition.y < m_boundingBox.top+m_boundingBox.height)
			{
				realCameraPos.y = newCameraPosition.y;
			}
		
			if(realCameraPos != currentPosition)
			{
				m_hasClicked = false;
				GE->SetCameraPosition(realCameraPos);
				GE->ApplyCamera();
			}
		
			m_mousePrevFrame = currentMouse.position;
		}
	}
}

void CameraSystem::DrawBoundingBox() const
{
	GE->SetColor(0, 1, 0);
	GE->DrawRect(m_boundingBox.left, m_boundingBox.top, m_boundingBox.width, m_boundingBox.height);
}

bool CameraSystem::HasClicked() const
{
	if(GE->GetMouse().left.upThisFrame)
	{
		return m_hasClicked;
	}
	
	return false;
}