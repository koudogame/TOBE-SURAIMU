#pragma once

// 板場 温樹

#include "object_base.h"

//-----------------------------------------------------------------------------
// 背景オブジェクト
//-----------------------------------------------------------------------------
// --説明--
// 画面外( 下 )へ出たら死亡判定
class BackObject :
    public ObjectBase
{
public:
    BackObject();
    ~BackObject();

    bool init(const wchar_t* const TextureName,
              const RECT& Trimming,
              const float ScrollX = 1.0F,
              const float ScrollY = 1.0F,
              const float DrawDepth = 0.0F);
    void destroy() override;
    void update() override;
    void draw() override;

    bool isAlive() override { return position_.y < getWindowHeight<float>(); }

    void resetStates(const float Magnification) { magnification_ = Magnification; }
    void reset(const RECT& Trimming,
               const float ScrollX, const float ScrollY,
               const float DrawDepth); 

private:
    bool created_        = false;
    RECT trimming_       { 0L,0L,0L,0L };
    float scroll_x_      = 1.0F;
    float scroll_y_      = 1.0F;
    float draw_depth_    = 0.0F;
    float magnification_ = 1.0F;

    float angle_         = 0.0F;
    void addAngle();
    void defAngle();
    void (BackObject::*set_angle_)()   = nullptr;

    float alpha_red_1    = 0.0F;
    float alpha_red_2    = 0.0F;
    void (BackObject::*alpha_blend_)() = nullptr;
    void addAlphaRed1();
    void addAlphaRed2();
};
