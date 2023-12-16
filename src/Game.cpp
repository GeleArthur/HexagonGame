#include "Game.h"
#include <iostream>
#include "GameEngine/GameEngine.h"

// Why???
Game *Game::_gamePtr{nullptr};

Game::Game() : _backGround()
{
	GE->SetWindowSize(1280, 720);
	GE->SetVsync(1);
	GE->SetTitle("Land VS Sea");
	
	_hexagonManager = new HexagonManager(50);
	_camera = new CameraSystem(Rect{-GE->GetWindowWidth(),-GE->GetWindowHeight(), GE->GetWindowWidth(), GE->GetWindowHeight()});
}

Game::~Game()
{
	delete _camera;
}

Game* Game::GetGamePtr()
{
	return _gamePtr;
}

void Game::Start()
{
	_hexagonManager->Start();
	_camera->Start();
	_gamePtr = this;
	GE->TextureFromFile("Background.png", _backGround);
}

void Game::Update()
{
	_camera->UpdateCamera();
	_hexagonManager->Update();
}

void Game::Draw()
{
	GE->ClearBackground(0.1f,0.1f,0.1f);
	GE->DrawTexture(_backGround, Vector2d{-GE->GetWindowWidth(), -GE->GetWindowHeight()});
	
	_hexagonManager->Draw();
}

void Game::DrawUI()
{
	_hexagonManager->DrawUi();
}

CameraSystem* Game::GetCamera() const
{
	return _camera;
}

HexagonManager * Game::GetHexagonManager() const
{
	return _hexagonManager;
}
