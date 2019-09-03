
// ”Âê

#pragma once

#include "view_background_base.h"

class ViewStarBig
    : public ViewBase
{
public:
    ViewStarBig();
    ~ViewStarBig();

    bool init( const Vector2& Position, const Color Color ) override;
    void destroy() override;
    void update() override;
    void draw() override;
};
