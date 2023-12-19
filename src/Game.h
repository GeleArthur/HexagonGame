#pragma once
#include "CameraSystem.h"
#include "HexagonManager.h"
#include "GameEngine/AbstractGame.h"

class Game : public AbstractGame
{
private:
    HexagonManager* _hexagonManager;
    CameraSystem* _camera;
    Texture _backGround;
    static Game *_gamePtr;

    
public:
    Game();
    ~Game() override;

    static Game* GetGame();
    
    void Start() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;

    CameraSystem* GetCamera() const;
    HexagonManager* GetHexagonManager() const;
};
