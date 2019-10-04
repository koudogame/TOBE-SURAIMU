
// 板場

#include "stage.h"

#include "task_manager.h"
#include "space.h"

#include "stage_data.h"
#include "player.h"
#include "star_container.h"
#include "wall.h"
#include "fail_wall.h"



// 定数
/*===========================================================================*/
static constexpr int kGamePad = 0; // ゲームパッドID
static constexpr float kScrollLineTop    = getWindowHeight<float>() * 0.3F;
static constexpr float kScrollLineBottom = getWindowHeight<float>() * 0.85F;


// Ctor, Dtor
/*===========================================================================*/
Stage::Stage()
{

}

Stage::~Stage()
{

}


// public
/*===========================================================================*/
// 初期化関数
// オブジェクトに関して、既にインスタンスを確保している場合は初期化処理のみを行っている
bool Stage::init( const std::wstring& DataFileName )
{
    // ステージデータの読み込み
    if (data_ == nullptr) { data_ = new StageData(); }
    if (data_->load(DataFileName) == false) { return false; }

    // プレイヤー初期化
    if (player_ == nullptr) { player_ = new Player(); }
    if (player_->init(data_->player_position, kGamePad) == false) { return false; }

    // スターコンテナ初期化
    if (stars_ == nullptr) { stars_ = new StarContainer(); }
    if (stars_->createStar(data_->star_pattern_file_name) == false) { return false; }

    // 壁初期化
    if (wall_ == nullptr) { wall_ = new Wall(); }
    if (wall_->init() == false) { return false; }

    // 炎初期化
    if (fire_ == nullptr) { fire_ = new FailWall(); }
    if (fire_->init() == false) { return false; }


    // メンバ変数の初期化
    phase_ = &Stage::phaseStart;
    scroll_count_ = 0.0F;

    return true;
}
/*===========================================================================*/
// 終了処理
// init関数でインスタンスの取得を行った順に開放していく
void Stage::destroy()
{
    // 炎開放
    if (fire_ != nullptr)
    {
        fire_->destroy();
        safe_delete(fire_);
    }
    // 壁開放
    if (wall_ != nullptr)
    {
        wall_->destroy();
        safe_delete(wall_);
    }
    // スターコンテナ開放
    if (stars_ != nullptr)
    {
        stars_->destroy();
        safe_delete(stars_);
    }
    // プレイヤー開放
    if (player_ != nullptr)
    {
        player_->destroy();
        safe_delete(player_);
    }
    // ステージデータ開放
    if (data_ != nullptr)
    {
        safe_delete(data_);
    }
}
/*===========================================================================*/
// 更新処理
// 現在のフェーズを実行する
bool Stage::update()
{
    return (this->*phase_)();
}
/*===========================================================================*/
// 描画処理
// オブジェクトの描画は、基本的にSpriteクラスが行う
void Stage::draw()
{

}


// フェーズ
/*===========================================================================*/
// スタート
// プレイヤーがジャンプしたら、フェーズをphasePlayへ
bool Stage::phaseStart()
{
    // プレイヤーのジャンプを確認
    if( player_->isJump() )
    {
        phase_ = &Stage::phasePlay;

        player_->onStartFlag();
        stars_->setFall();
        fire_->start();
    }


    // 衝突処理
    Space::getInstance()->collision();

    return true;
}
/*===========================================================================*/
// プレイ
// プレイヤーがゴールしたら、フェーズをphaseGoalへ
// プレイヤーが死亡したら更新終了
bool Stage::phasePlay()
{
    // プレイヤーの死亡を確認
    if( player_->isAlive() == false )
    {
        return false;
    }


    // スターコンテナの更新
    stars_->update();

    // スクロール処理
    scroll();

    // 衝突処理
    Space::getInstance()->collision();



    // ゴール
    if( isGoaled() ) 
        phase_ = &Stage::phaseGoal;

    return true;
}
/*===========================================================================*/
// ゴール
// 処理が終了したら、更新終了
bool Stage::phaseGoal()
{
    return false;
}


/*===========================================================================*/
// スクロール処理
void Stage::scroll()
{
    // プレイヤーの座標
    const Vector2& player_position = player_->getPosition();


    // スクロール範囲に入った分の値を求める
    float over = 0.0F;
    if( player_position.y < kScrollLineTop )
    {
        over = player_position.y - kScrollLineTop;
    }
    else if( player_position.y > kScrollLineBottom )
    {
        over = player_position.y - kScrollLineBottom;
    }
    scroll_count_ += over;


    // タスクマネージャーに登録している全オブジェクトに移動処理を実行させる
    TaskManager::getInstance()->allSetOver( -over );
}


/*===========================================================================*/
// ゴール状況を取得する
// return true  : ゴールしている 
// return false : ゴールいていない
bool Stage::isGoaled() const
{
    float position = player_->getPosition().y + scroll_count_;

    return position < data_->goal_line;
}
