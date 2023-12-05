#include <iostream>
#include <string>
#include <ctime>

#include "../Game.h"
#include "GameEngine.h"

#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"Glu32.lib")


int main(int argc, char* args[])
{
	srand((unsigned int)time(nullptr));

	GameEngine* engine = new GameEngine();
	engine->SetGame(new Game());
	engine->Initialize();
	engine->Run();
	engine->CleanUp();

	return 0;
}

