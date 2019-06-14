#pragma once

#include "player.h"

class Sercher;

//-----------------------------------------------------------------------------
// デモプレイ用AI
//-----------------------------------------------------------------------------
// --説明--
// とにかく近くの星へ飛び移る
class AIDemo :
    public Player
{
public:
    AIDemo();
    ~AIDemo();

    bool init( const Vector2& Position, const int PlayerNo = 0 ) override;
    void destroy() override;
    void update() override;

private:
    //void input(  ) override;

    bool is_jumping_        = false;
    Sercher *sercher_       = nullptr;
    ObjectBase *purpose_    = nullptr;
};
