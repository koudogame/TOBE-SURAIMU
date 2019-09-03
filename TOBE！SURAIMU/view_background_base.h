#pragma once

#include "back_object_base.h"


//-----------------------------------------------------------------------------
// 背景基底クラス( 波以外 )
//-----------------------------------------------------------------------------
// --説明--
// init関数でメンバの初期化を必ず行うこと
class ViewBase
    : public BackObjectBase
{
public:
    virtual ~ViewBase() = default;

    virtual void setMove( const float Offset ) override { scroll(Offset); }
    virtual bool isAlive() override { return position_.y < getWindowHeight<float>(); }


    const Vector2& getPosition() const { return position_; }

    void drawSeamless( const RECT& Trimming, 
                       const float DrawDepth,
                       const float Alpha = 1.0F );

protected:
    void scroll( const float Offset );

    Vector2 position_ {0.0F, 0.0F};
};
