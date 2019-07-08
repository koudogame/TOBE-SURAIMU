
// î¬èÍÅ@â∑é˜

#pragma once

#include "ai_base.h"

class Sercher;

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
    bool isMoveLeft()  override;
    bool isMoveRight() override;
    bool isMoveDown()  override;

    void setPurpose();

    bool checkPurposeForUp( ObjectBase* const Target );
    bool checkPurposeForDown( ObjectBase* const Target );
    bool checkPurposeForGround( ObjectBase* const Target );

    Sercher    *sercher_ = nullptr;
    ObjectBase *purpose_ = nullptr;
};
