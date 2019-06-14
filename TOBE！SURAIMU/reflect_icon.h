
// 板場 温樹

#pragma once

#include "bonus_icon_base.h"


//-----------------------------------------------------------------------------
// ボーナスアイコン : 「リフレクト」
//-----------------------------------------------------------------------------
class ReflectIcon :
    public BonusIconBase
{
public:
    ReflectIcon();
    ~ReflectIcon();

    bool init( const Vector2& Position ) override;
    void destroy() override;
    void update()  override;
    void draw()    override;

private:
};
