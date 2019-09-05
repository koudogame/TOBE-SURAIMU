
// î¬èÍ

#pragma once

#include "back_object_base.h"

class ViewStarMini
    : public BackObjectBase
{
public:
    ViewStarMini();
    ~ViewStarMini();

    bool init( const Vector2& Position, const Color Color ) override;
    void destroy() override;
    void update() override;
    void draw() override;
};
