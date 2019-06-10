#pragma once

#include "object_base.h"
#include "shape.h"

class Star;

class StarSercher :
    public ObjectBase
{
public:
    StarSercher() = default;
    StarSercher( const Vector2& Origin, const float Radius )
    {
        set( Origin, Radius );
    }


    bool init();
    void update();
    void draw() {}

    void collision( Star* const Object ) { stars_.push_back(Object); }


// ƒQƒbƒ^[
    Circle* getShape()                        { return &range_; }
    const Vector2& getOrigin() const          { return getShape()->position; }

    const std::deque<Star*>& getStars() const { return stars_; }

private:
    std::deque<Star*> stars_;
    Circle range_ = { {0.0F, 0.0F}, 0.0F };
};