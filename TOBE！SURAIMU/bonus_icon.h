#pragma once

#include "object_base.h"

//-----------------------------------------------------------------------------
// ボーナスアイコン
//-----------------------------------------------------------------------------
class BonusIcon :
    public ObjectBase
{
public:
    BonusIcon();
    virtual ~BonusIcon();


    virtual bool init( const wchar_t* const TextureFileName,
                       const Vector2& Position );
    virtual void destroy() override;
    virtual void update()  override;
    virtual void draw() override;

    virtual bool isAlive() { return (position_.y < getWindowHeight<float>()); }
        // 画面外で false を返却

// セッター
    virtual void setMove( const float Disp )            { position_.y += Disp;}
// ゲッター
    virtual const Vector2& getPosition() const override { return position_; }

protected:
    virtual void animation();

    ID3D11ShaderResourceView* texture_ = nullptr;
    Vector2 position_ { 0.0F, 0.0F };

    int anim_frame_ = 0;
    int anim_count_ = 0;
};