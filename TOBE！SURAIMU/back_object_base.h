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
    virtual void draw() override = 0;

    virtual void setMove( const float ) override = 0;
    virtual bool isAlive() override = 0;

    virtual void setColor( const Color Color ) { color_ = Color; }


protected :
    ID3D11ShaderResourceView *texture_ = nullptr;
    Color color_ = kPurple;
};
