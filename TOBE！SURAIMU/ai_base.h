
// ”Âê@‰·÷

#pragma once

#include "player.h"

class AIBase :
    public Player
{
public:
    virtual ~AIBase() = default;

    virtual bool init( const Vector2& Position, const int PlayerNo = 0 ) override = 0;
    virtual void destroy() override = 0;
    virtual void update() override  = 0;

protected:
    virtual bool isSquat() = 0;
    virtual bool isJump()  = 0;
    virtual bool isMoveLeft()  = 0;
    virtual bool isMoveRight() = 0;
    virtual bool isMoveDown()  = 0;

private:
    void inputjump() override final;
    void inputmove() override final;
    void jump();
    void move( int Way );
};
