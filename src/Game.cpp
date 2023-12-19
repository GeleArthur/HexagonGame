#include "Game.h"
#include <iostream>
#include "GameEngine/GameEngine.h"

// Why???
Game *Game::m_gamePtr{nullptr};

Game::Game() : m_backGround()
{
	GE->SetWindowSize(1280, 720);
	GE->SetVsync(1);
	GE->SetTitle("Land VS Sea");
	
	m_hexagonManager = new HexagonManager(50);
	m_camera = new CameraSystem(Rect{-GE->GetWindowWidth(),-GE->GetWindowHeight(), GE->GetWindowWidth(), GE->GetWindowHeight()});
}

Game::~Game()
{
	delete m_camera;
	delete m_hexagonManager;
}

Game* Game::GetGame()
{
	return m_gamePtr;
}

void Game::Start()
{
	m_hexagonManager->Start();
	m_camera->Start();
	m_gamePtr = this;
	GE->TextureFromFile("Background.png", m_backGround);
}

void Game::Update()
{
	m_camera->UpdateCamera();
	m_hexagonManager->Update();
}

void Game::Draw()
{
	GE->ClearBackground(0.1f,0.1f,0.1f);
	GE->DrawTexture(m_backGround, Vector2d{-GE->GetWindowWidth(), -GE->GetWindowHeight()});
	
	m_hexagonManager->Draw();
}

void Game::DrawUi()
{
	m_hexagonManager->DrawUi();
}

CameraSystem* Game::GetCamera() const
{
	return m_camera;
}

HexagonManager * Game::GetHexagonManager() const
{
	return m_hexagonManager;
}
