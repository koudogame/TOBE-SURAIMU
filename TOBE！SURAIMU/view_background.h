#pragma once

#include "back_object_base.h"

class View
    : public BackObjectBase
{
public:
    View();
    ~View();

    bool init( const Vector2& Position, const Color Color );
    void destroy();
    void update();
    void draw();
    void setMove( const float Offset ) override { offset_y_ += Offset; } 
    bool isAlive() override { return position_[0].y < getWindowHeight<float>(); }

private:
    Vector2 *position_ = nullptr;

    float offset_y_ = 0.0F;
};
