
// 板場　温樹

#pragma once

#include "player.h"

// AI基底クラス
//
// is~~関数の戻り値を確認して各関数名にある行動をとる
class AIBase :
    public Player
{
public:
    virtual ~AIBase() = default;

    virtual bool init( const Vector2& Position, const int PlayerNo = 0 ) override = 0;
    virtual void destroy() override = 0;
    virtual void update() override  = 0;

protected:
    virtual bool isSquat() { return false; }
    virtual bool isJump()  { return false; }
    virtual bool isMoveLeft()  { return false; }
    virtual bool isMoveRight() { return false; }
    virtual bool isMoveDown()  { return false; }

private:
    void inputjump() override final;
    void inputmove() override final;
    void jump();
    void move( int Way );
};
