#pragma once

// 板場 温樹

#include "object_base.h"

namespace {
    constexpr float kPercentegeForMove = 0.5F;
}

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

    bool init(const RECT& Trimming,
              const float ScrollX = 1.0F,
              const float ScrollY = 1.0F,
              const float DrawDepth = 0.0F);
    void destroy() override;
    void update() override;
    void draw() override;

    bool isAlive() override { return position_.y < getWindowHeight<float>(); }
    void setMove(const float Move) override { position_.y += Move * kPercentegeForMove; } 

    void resetStates(const float Magnification) { magnification_ = Magnification; }
    void reset(const RECT& Trimming,
               const float ScrollX, const float ScrollY,
               const float DrawDepth); 

private:
    bool created_         = false;

    ID3D11ShaderResourceView* texture_sub_ = nullptr;

    RECT trimming_        { 0L,0L,0L,0L };
    float scroll_x_       = 0.0F;
    float scroll_y_       = 0.0F;
    float draw_depth_     = 0.0F;
    float magnification_  = 0.0F;

    bool is_add_subalpha_ = false;
    float sub_alpha_      = 0.0F;
};
