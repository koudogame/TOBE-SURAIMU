
// 板場

#pragma once

#include "scene_base.h"

// ステージモード「ステージセレクト」シーン
// 遷移先は「タイトル」「ステージプレイ」
class StageSelect :
    public SceneBase
{
public:
    StageSelect();
    ~StageSelect();

// override
    bool init() override;
    void destroy() override;
    SceneBase* update() override;
    void draw() override;

private:
    bool selectStage();
    void offsetting();


    Vector2 draw_position_;
    unsigned selecting_stage_ = 0U;
    Vector2 rest_offset_;
};
