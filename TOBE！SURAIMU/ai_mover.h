#pragma once

#include "player.h"

class Sercher;

//-----------------------------------------------------------------------------
// AI
//-----------------------------------------------------------------------------
// --説明--
// 上に飛んだ後、目的のスターへ移動する
class AIMover :
    public Player
{
public:
    AIMover();
    ~AIMover();

    bool init( const Vector2& Position, const int PlayerNo = 0 ) override;
    void destroy() override;
    void update() override;

private:
    void inputjump() override;
    void inputmove() override;

    float getRandJumpAngle();

    Sercher *sercher_      = nullptr;
    ObjectBase *purpose_   = nullptr;
    float jump_angle_ = 0.0F;
};
