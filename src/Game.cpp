#include "Game.h"
#include <iostream>
#include <SDL_keyboard.h>
#include "GameEngine/GameEngine.h"



Game::Game() : _world(50)
{
	GE->SetWindowSize(640, 640);
	GE->SetVsync(1);
	GE->SetTitle("Land VS Sea");
}

void Game::Start()
{
	_world.Start();
	GE->EnableCameraDrag(true);
}

void Game::Update()
{

}

void Game::Draw()
{
	GE->ClearBackground(0.1f,0.1f,0.1f);
	
	_world.Draw();
}

