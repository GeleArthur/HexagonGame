#pragma once
#include "CameraSystem.h"
#include "HexagonManager.h"
#include "GameEngine/AbstractGame.h"

class Game : public AbstractGame
{
private:
    HexagonManager _world;
    CameraSystem _camera;
    Texture _backGround;

    
    
public:
    Game();
    void Start() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
};
