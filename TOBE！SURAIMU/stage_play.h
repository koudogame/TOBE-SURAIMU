
// 板場

#pragma once

#include "scene_base.h"

class Stage;

// ステージプレイクラス
class StagePlay :
    public SceneBase
{
public:
    StagePlay( const std::wstring& StageDataFileName );
    ~StagePlay();

    // ステージ初期化処理
    // 戻り値 [ true : 正常終了　false : エラー ]
    bool init() override;
    void destroy() override;
    // ステージ更新処理
    // 戻り値 : 遷移先シーンへのポインタ( init関数は呼んでいない )
    SceneBase* update() override;
    void draw() override;


private:
    std::wstring stage_data_name_;
    Stage *stage_ = nullptr;
};
