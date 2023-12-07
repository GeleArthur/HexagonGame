#pragma once
#include "Hexagons/Hexagon.h"
#include "Hexagons/HexagonManager.h"
#include "Utils/Vector2d.h"
#include "GameEngine/AbstractGame.h"

class Game : public AbstractGame
{
private:
    HexagonManager _world;
    
public:
    Game();
    void Start() override;
    void Update() override;
    void Draw() override;
};
