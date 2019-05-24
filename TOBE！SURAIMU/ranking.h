#pragma once

// 板場 温樹

#include "scene_base.h"

//-----------------------------------------------------------------------------
// ランキングシーン
//-----------------------------------------------------------------------------
class Ranking :
    public SceneBase
{
public:
    Ranking();
    ~Ranking();

    bool init() override;
    void destroy() override;
    SceneBase* update() override;
    void draw() override;

private:
    bool created_ = false;

    float magnification_ = 1.0F;
    float offset_ = 0.0F;
};
