#include "Game.h"
#include <iostream>
#include "GameEngine/GameEngine.h"

// Should change :(
Game::Game() : _world(50), _backGround()
{
	GE->SetWindowSize(1280, 720);
	GE->SetVsync(1);
	GE->SetTitle("Land VS Sea");
	_camera = new CameraSystem(Rect{-GE->GetWindowWidth(),-GE->GetWindowHeight(), GE->GetWindowWidth(), GE->GetWindowHeight()});
}

Game::~Game()
{
	delete _camera;
}

void Game::Start()
{
	_world.Start();
	_camera->Start();
	GE->TextureFromFile("Background.png", _backGround);
}

void Game::Update()
{
	_camera->UpdateCamera();
	_world.Update();
}

void Game::Draw()
{
	GE->ClearBackground(0.1f,0.1f,0.1f);
	GE->DrawTexture(_backGround, Vector2d{-GE->GetWindowWidth(), -GE->GetWindowHeight()});
	
	_world.Draw();
}

void Game::DrawUI()
{
	_world.DrawUi();
	
}

