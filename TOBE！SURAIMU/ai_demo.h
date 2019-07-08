
// 板場　温樹

#pragma once

#include "ai_base.h"

class Sercher;

// デモ用プレイ(第1号)
//
// 画面内にある星に飛び移る
class AIDemo :
    public AIBase
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

    void setPurpose();

    bool checkPurposeForUp( ObjectBase* const Target );
    bool checkPurposeForDown( ObjectBase* const Target );
    bool checkPurposeForGround( ObjectBase* const Target );

    Sercher    *sercher_ = nullptr;
    ObjectBase *purpose_ = nullptr;
};
