#pragma once

#include "object_base.h"

//-----------------------------------------------------------------------------
// ボーナスアイコン基底
//-----------------------------------------------------------------------------
class BonusIconBase :
    public ObjectBase
{
public:
    BonusIconBase() = default;
    virtual ~BonusIconBase()  = default;


    virtual bool init( const Vector2& Position ) = 0;
    virtual void draw() override                 = 0;

    virtual bool isAlive() { return (position_.y < getWindowHeight<float>()); }
        // 画面外で false を返却

// セッター
    virtual void setMove( const float Disp )            { position_.y += Disp;}
// ゲッター
    virtual const Vector2& getPosition() const override { return position_; }

protected:
    ID3D11ShaderResourceView* texture_ = nullptr;
    Vector2 position_ { 0.0F, 0.0F };
};