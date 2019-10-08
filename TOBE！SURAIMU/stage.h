
// 板場

#pragma once

struct StageData;

class Player;
class StarContainer;
class Wall;
class FailWall;

class Stage
{
public:
    Stage();
    virtual ~Stage();

    // 初期化関数
    // 
    // 引数 : ステージデータ (フォーマットは構造体に従う)
    //
    // 戻り値 : 終了状況[ true : 正常終了　false : エラー ]
    virtual bool init( const std::wstring& StageDataFile );
    // 終了処理
    // 
    // 引数 : 無し
    //
    // 戻り値 : 無し
    virtual void destroy();
    // 更新処理
    //
    // 引数 : 無し
    //
    // 戻り値 : 更新継続の可否[ true : 更新継続　false : 更新終了 ]
    virtual bool update();
    // 描画処理
    //
    // 引数 : 無し
    //
    // 戻り値 : 無し
    virtual void draw();

protected:
    virtual bool phaseStart();
    virtual bool phasePlay();
    virtual bool phaseGoal();

    virtual float scroll();

    virtual bool isGoaled() const;


    bool (Stage::*phase_)() = nullptr;  // 現在のフェーズ

    ID3D11ShaderResourceView *texture_ = nullptr; // テクスチャハンドル
    StageData     *data_   = nullptr;   // ステージデータ

    Player        *player_ = nullptr;   // プレイヤー
    StarContainer *stars_  = nullptr;   // スターコンテナ
    Wall          *wall_   = nullptr;   // 壁
    FailWall      *fire_   = nullptr;   // 炎

    float scroll_count_ = 0.0F;
};
