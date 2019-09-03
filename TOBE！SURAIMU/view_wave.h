#pragma once

#include "back_object_base.h"

class ViewWave 
    : public BackObjectBase
{
public:
    ViewWave();
    ~ViewWave();

    bool init( const Vector2& Position, const Color Color ) override;
    void destroy() override;
    void update() override;
    void draw() override;

    void setMove( const float Offset ) override { position_.y += Offset; }
    bool isAlive() override { return position_.y < getWindowHeight<float>(); }

    void setColor( const Color Color ) override;

private:
    void (ViewWave::*effect_)() = nullptr;
    void effectFadeIn();
    void effectFadeOut();

    Vector2 position_;
    Vector2 offset_;
    int pattern_id_ = 0;
    float effect_alpha_ = 0.0F;
};
