
// î¬èÍ

#pragma once

#include "back_object_base.h"

class ViewStarBig
    : public BackObjectBase
{
public:
    ViewStarBig();
    ~ViewStarBig();

    bool init( const Vector2& Position, const Color Color ) override;
    void destroy() override;
    void update() override;
    void draw() override;
};
