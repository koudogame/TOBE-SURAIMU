
// 板場

#pragma once

#include "scene_base.h"

// ステージプレイ->「リザルト」
//
// コンストラクタでクリア情報をセットする

class StageResult :
    public SceneBase
{
public:
    struct Info
    {
        int clear_time_sec = 0;
    };

    StageResult( const Info ClearInfo );
    ~StageResult();


    // 初期化処理
    // ! ステージクリア情報の設定はコンストラクタで行う !
    //
    // 戻り値[ true : 正常終了　false : エラー ]
    bool init() override;
    void destroy() override;
    SceneBase* update() override;
    void draw() override;


private:
    Info clear_info_;
};
