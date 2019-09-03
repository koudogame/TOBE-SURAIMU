
// ”Âê

#pragma once

#include "view_background_base.h"

class ViewMist 
    : public ViewBase
{
public:
    ViewMist();
    ~ViewMist();

    bool init( const Vector2& Position, const Color Color ) override;
    void destroy() override;
    void update() override;
    void draw() override;
};
