#pragma once
#include "CameraSystem.h"
#include "HexagonManager.h"
#include "GameEngine/AbstractGame.h"

class Game : public AbstractGame
{
private:
    HexagonManager* m_hexagonManager;
    CameraSystem* m_camera;
    Texture m_backGround;
    static Game *m_gamePtr;

    
public:
    Game();
    ~Game() override;

    static Game* GetGame();
    
    void Start() override;
    void Update() override;
    void Draw() override;
    void DrawUi() override;

    CameraSystem* GetCamera() const;
    HexagonManager* GetHexagonManager() const;
};
