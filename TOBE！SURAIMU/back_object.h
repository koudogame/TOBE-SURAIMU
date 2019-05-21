#pragma once

// 板場 温樹

#include "object_base.h"

//-----------------------------------------------------------------------------
// 背景オブジェクト
//-----------------------------------------------------------------------------
// --説明--
// y座標が画面外へ出たら死亡判定
class BackObject :
    public ObjectBase
{
public:
    BackObject(TaskManager* const TaskManager);
    ~BackObject();

    bool init(const RECT& Trimming, const float Scroll);
    void destroy() override;
    void update() override;
    void draw() override;

    bool isAlive() override { return position_.y <= getWindowHeight<float>(); }

    void resetStates(const float Magnification) { magnification_ = Magnification; }
    void reset(const RECT& Trimming); 

private:
    bool created_ = false;
    RECT trimming_{ 0L,0L,0L,0L };
    float scroll_ = 1.0F;
    float magnification_ = 1.0F;

    float alpha_red_1 = 0.0F;
    float alpha_red_2 = 0.0F;
    void (BackObject::*alpha_blend_)() = nullptr;
    void addAlphaRed1();
    void addAlphaRed2();
};
