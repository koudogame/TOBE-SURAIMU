#pragma once

#include "back_object_base.h"

class Wave 
    : public BackObjectBase
{
public:
    Wave();
    ~Wave();

    bool init( const Vector2& Position, const Color Color ) override;
    void destroy() override;
    void update() override;
    void draw() override;

    void setMove( const float Offset ) override { position_.y += Offset; }
    bool isAlive() override { return position_.y < getWindowHeight<float>(); }

private:
    Vector2 position_;
    Vector2 offset_;
};
