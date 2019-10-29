
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

    /* 初期化処理
        
        param StageDataFile : ステージデータファイル名
        param pPlayer       : 生成したプレイヤーのアドレス( ステージでの値変更はない )
        param Border        : ステージの境界線( 実際のスタートライン )

        return true  : 正常終了
        return false : エラー
    */
    virtual bool init( 
        const std::wstring& StageDataFile,
        Player* const pPlayer,
        const float StartLine
    );
    /* 初期化処理    スタートラインは読み込むデータ通り
        
        param StageDataFile : ステージデータファイル名
        param pPlayer       : 生成したプレイヤーのアドレス( ステージでの値変更はない )
    */
    virtual bool init(
        const std::wstring& StageDataFile,
        Player* const pPlayer
    );
    /* 終了処理
    */
    virtual void destroy();
    /* 更新処理

        return true  : 更新継続
        return false : 更新終了( ゴールした )
    */
    virtual bool update();
    /* 描画処理
    */
    virtual void draw();
    /* スタートする
    */
    virtual void start();
    /* ステージデータの取得

        return : ステージデータのconst参照
    */
    const StageData& getStageData() const { return *data_; }
    /* 進行度の取得

        param PlayerPositionLine : プレイヤーの現在位置

        return : ステージ全体を100%としてみた時の、プレイヤーの現在位置の割合
    */
    float getProgress() const;

protected:
    virtual bool phaseStart();
    virtual bool phasePlay();
    virtual float scroll();
    virtual bool isGoaled() const;


    bool (Stage::*phase_)() = nullptr;  // 現在のフェーズ
    StageData     *data_   = nullptr;   // ステージデータ
    StarContainer *stars_  = nullptr;   // スターコンテナ
    Player  *player_ = nullptr;         // プレイヤー
    float start_line_ = 0.0F;           // ステージのスタートライン(スクロールに合わせて動く)
};
