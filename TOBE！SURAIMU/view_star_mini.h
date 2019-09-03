#pragma once

#include "view_background_base.h"

class ViewStarMini
    : public ViewBase
{
public:
    ViewStarMini();
    ~ViewStarMini();

    bool init( const Vector2& Position, const Color Color ) override;
    void destroy() override;
    void update() override;
    void draw() override;
};
