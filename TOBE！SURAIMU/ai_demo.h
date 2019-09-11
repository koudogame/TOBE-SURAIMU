
// 板場

#pragma once

#include "ai_base.h"

class Sercher;
class Star;

//-----------------------------------------------------------------------------
// デモプレイ用CPU
//-----------------------------------------------------------------------------
// --説明--
// 画面内にあるスターへ飛び移る
class AIDemo
    : public AIBase

{
public:
    AIDemo();
    ~AIDemo();

    bool init( const Vector2& Position, const int PlayerNo = 0 ) override;
    void destroy() override;
    void update()  override;

private:
    bool isSquat() override;
    bool isJump()  override;
    bool isMoveLeft()  override;
    bool isMoveRight() override;
    bool isMoveDown()  override;

    void setTarget();

    bool (AIDemo::*getJudgeFunc())(ObjectBase* const);
        bool judgeForStaying( ObjectBase* const );
        bool judgeForJumping( ObjectBase* const );
        bool judgeForFalling( ObjectBase* const );


    Sercher *sercher_ = nullptr;
    Star    *target_ = nullptr;

    Vector2 last_position_;
    bool is_jumping_ = false;
};
