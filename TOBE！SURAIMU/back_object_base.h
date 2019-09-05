
// î¬èÍ

#pragma once

#include "object_base.h"

class BackObjectBase
    : public ObjectBase
{
public :
    enum Color : int
    {
        kPurple,
        kBlue,
        kRed,
    };

    BackObjectBase() = default;
    virtual ~BackObjectBase() = default;

    virtual bool init( const Vector2& Position, const Color Colro ) = 0;

    virtual bool isAlive() override { return is_alive_; }
    virtual const Vector2& getPosition() const override { return position_; }
    virtual void setMove( const float Distance ) override { position_.y += Distance; };
    virtual void setColor( const Color Color ) { color_ = Color; }


protected :
    ID3D11ShaderResourceView *texture_ = nullptr;
    Vector2 position_;
    Color color_ = kPurple;
    bool is_alive_ = false;
};
