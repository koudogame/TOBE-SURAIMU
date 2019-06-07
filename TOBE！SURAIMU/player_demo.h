#pragma once

#include "player.h"

class Star;

//-----------------------------------------------------------------------------
// デモプレイ用AI
//-----------------------------------------------------------------------------
// --説明--
// とにかく近くの星へ飛び移る
class DemoPlayer :
    public Player
{
private:
    void input() override;

    void serchStar();

    Star* purpose_ = nullptr;
};