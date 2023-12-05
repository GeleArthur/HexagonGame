#pragma once
#include <string>

class AbstractGame
{
private:
    
public:
    AbstractGame();
    virtual ~AbstractGame();
    virtual void Start() ;
    virtual  void Update();
    virtual  void Draw();
};