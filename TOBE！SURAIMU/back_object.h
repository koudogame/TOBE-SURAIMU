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

    bool init(const RECT& Trimming,
              const float Scroll = 1.0F,
              const float DrawDepth = 0.0F);
    void destroy() override;
    void update() override;
    void draw() override;

    bool isAlive() override;

    void resetStates(const float Magnification) { magnification_ = Magnification; }
    void reset(const RECT& Trimming); 

private:
    bool created_ = false;
    RECT trimming_{ 0L,0L,0L,0L };
    float scroll_ = 1.0F;
    float draw_depth_ = 0.0F;
    float magnification_ = 1.0F;

    float angle_ = 0.0F;
    void addAngle();
    void defAngle();
    void (BackObject::*set_angle_)() = nullptr;

    float alpha_red_1 = 0.0F;
    float alpha_red_2 = 0.0F;
    void (BackObject::*alpha_blend_)() = nullptr;
    void addAlphaRed1();
    void addAlphaRed2();
};
