
// 板場

#include "stage.h"

#include "task_manager.h"

#include "stage_data.h"
#include "player.h"
#include "star_container.h"


// TODO : テクスチャを使用してラインが正しく動いているか確認する



// 定数
/*===========================================================================*/
static constexpr int kGamePad = 0; // ゲームパッドID
static constexpr float kFallSpeed = 2.0F;


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
bool Stage::init( const std::wstring& DataFileName, Player* const pPlayer, const float StartLine )
{
    TaskManager::getInstance()->registerTask( this, TaskUpdate::kWall );

    // ステージデータの読み込み
    if (data_ == nullptr) { data_ = new StageData(); }
    if (data_->load(DataFileName) == false) { return false; }

    // スターコンテナ初期化
    if (stars_ == nullptr) { stars_ = new StarContainer(); }
    float offset = StartLine - data_->start_line;
    if (stars_->createStar(data_->star_pattern_file_name, offset) == false) { return false; }

    // メンバ変数の初期化
    phase_ = &Stage::phaseStart;
    player_ = pPlayer;
    start_line_ = StartLine;

    return true;
}
// 初期化処理
bool Stage::init( const StageData& Data, Player* const pPlayer )
{
    // タスクマネージャーに登録
    TaskManager::getInstance()->registerTask( this, TaskUpdate::kWall );

    if( data_ == nullptr ) { data_ = new StageData(); }
    *data_ = Data;

    // スターコンテナ初期化
    if (stars_ == nullptr) { stars_ = new StarContainer(); }
    if (stars_->createStar(data_->star_pattern_file_name) == false) { return false; }

    // メンバ変数の初期化
    phase_ = &Stage::phaseStart;
    player_ = pPlayer;
    start_line_ = data_->start_line;

    return true;
}
/*===========================================================================*/
// 終了処理
// init関数でインスタンスの取得を行った順に開放していく
void Stage::destroy()
{
    // スターコンテナ開放
    if (stars_ != nullptr)
    {
        stars_->destroy();
        safe_delete(stars_);
    }
    // ステージデータ開放
    if (data_ != nullptr)
    {
        safe_delete(data_);
    }

    // プレイヤーとの依存関係を断つ
    player_ = nullptr;

    // タスクマネージャーから登録を解除
    TaskManager::getInstance()->unregisterObject( this );
}
/*===========================================================================*/
// 更新処理
// 現在のフェーズを実行する
void Stage::update()
{
    (this->*phase_)();
}
/*===========================================================================*/
// 描画処理
// オブジェクトの描画は、基本的にSpriteクラスが行う
void Stage::draw()
{
}
/*===========================================================================*/
// スタート
void Stage::start()
{
    phase_ = &Stage::phasePlay;

    stars_->setFall();
}
/*===========================================================================*/
// 進行度返却
float Stage::getProgress() const
{
    float displacement = start_line_ - player_->getPosition().y;

    return displacement / data_->height;
}
/*===========================================================================*/
// ゴールラインの取得
float Stage::getGoalLine() const 
{
    return start_line_ - data_->height;
}


// フェーズ
/*===========================================================================*/
// スタート
bool Stage::phaseStart()
{
    return true;
}
/*===========================================================================*/
// プレイ
// プレイヤーがゴールしたら更新終了
// プレイヤーが死亡したら更新終了
bool Stage::phasePlay()
{
    // スターコンテナの更新
    stars_->update();

    start_line_ += kFallSpeed;
    
    
    // ゴールを確認
    if( isGoaled() )
    {
        return false;
    }
    // プレイヤーの死亡を確認
    if( player_->isAlive() == false )
    {
        return false;
    }


    return true;
}

/*===========================================================================*/
// ゴール状況を取得する
// return true  : ゴールしている 
// return false : ゴールいていない
bool Stage::isGoaled() const
{
    float distance = start_line_ - player_->getPosition().y;

    return distance >= data_->height;
}
