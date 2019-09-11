
// ”Âê

#pragma once

#include "back_object_base.h"

class ViewMist 
    : public BackObjectBase
{
public:
    ViewMist();
    ~ViewMist();

    bool init( const Vector2& Position, const Color Color ) override;
    void destroy() override;
    void update() override;
    void draw() override;
};
