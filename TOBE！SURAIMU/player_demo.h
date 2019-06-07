#pragma once

#include "player.h"
#include "star.h"

class SerchRangeForStar :
    public ObjectBase
{
public:
    void destroy() {}
    void update() {}
    void draw() {}


    SerchRangeForStar() = default;
    SerchRangeForStar( const Vector2& RangeOrigin, const float RangeRadius )
    {
        set( RangeOrigin, RangeRadius );
    }

    void collision( Star* const Star ) { target_ = dynamic_cast<ObjectBase*>(Star); }

    void set( const Vector2& Origin, const float Radius)
    {
        range_.position = Origin;
        range_.radius = Radius;
    }
    void setOrigin( const Vector2& Origin ) { range_.position = Origin; }
    void setRadius( const float Radius )    { range_.radius = Radius; }

    Circle* getShape() { return &range_; }
    ObjectBase* getDiscStar() { return target_; }

private:
    ObjectBase* target_     = nullptr;
    Circle range_   { {0.0F, 0.0F}, 0.0F };
};


//-----------------------------------------------------------------------------
// デモプレイ用AI
//-----------------------------------------------------------------------------
// --説明--
// とにかく近くの星へ飛び移る
class DemoPlayer :
    public Player
{
private:
    void input() override;

    ObjectBase* serch();

    SerchRangeForStar serch_range_;
    ObjectBase* purpose_ = nullptr;
};