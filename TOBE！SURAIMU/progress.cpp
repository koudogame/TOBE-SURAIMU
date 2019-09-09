
// 板場

#include "progress.h"

#include "task_manager.h"
#include "player.h"
#include "fail_wall.h"
#include "progress_stage.h"
#include "progress_player.h"
#include "progress_fail_wall.h"


static constexpr float kToRadians = XM_PI / 180.0F;
static constexpr float toRadians( const float Degrees )
{
    return Degrees * kToRadians;
}


// 定数
/*===========================================================================*/
// 進行度用
static constexpr float kScrollSinMinDeg = 0.0F;
static constexpr float kScrollSinMaxDeg = 90.0F;
static constexpr float kScrollSinChange = 3.0F;
// プレイヤー用
static constexpr float   kPlayerStartLine    = 617.0F;
static constexpr float   kPlayerEndLine      = 107.0F;
static constexpr Vector2 kPlayerInitPosition { 295.0F, kPlayerStartLine };
static constexpr float   kPlayerMotionRange  = kPlayerStartLine - kPlayerEndLine;
// 炎用
static constexpr Vector2 kFailWallInitPosition { 260.0F, 720.0F };
// ステージ用
static constexpr Vector2 kStageInitPosition { 260.0F, -1440.0F };
static constexpr float   kStageMotionrange   = 720.0F;


// Ctor, Dtor
/*===========================================================================*/
Progress::Progress()
{

}

Progress::~Progress()
{

}


// override
/*===========================================================================*/
// 初期化処理
bool Progress::init( const float StageHeight,
                     Player* const Player,
                     FailWall* const FailWall )
{
    // タスクの登録
    TaskManager* const kTaskManager = TaskManager::getInstance();
    kTaskManager->registerTask( this, TaskUpdate::kRanking );
    kTaskManager->registerTask( this, TaskDraw::kObject );

    
    // ステージ初期化
    if( stage_ == nullptr ) { stage_ = new ProgressStage(); }
    if( stage_->init( kStageInitPosition ) == false )
    {
        return false;
    }

    // プレイヤー初期化
    float scale = kPlayerMotionRange / StageHeight;
    if( player_ == nullptr ) { player_ = new ProgressPlayer(); }
    if( player_->init( Player, 
                       kPlayerInitPosition,
                       kPlayerStartLine, 
                       kPlayerEndLine, 
                       scale ) == false
       )
    {
        return false;
    }

    // 炎初期化
    if( fail_wall_ == nullptr ) { fail_wall_ = new ProgressFailWall(); }
    if( fail_wall_->init( FailWall, Player, player_,
                          kFailWallInitPosition,
                          scale ) == false 
       )
    {
        return false;
    }
                          
    
    // その他メンバの初期化
    scroll_sin_ = -1.0F;


    return true;
}
/*===========================================================================*/
// 終了処理
void Progress::destroy()
{
    // 炎開放
    if( fail_wall_ != nullptr )
    {
        fail_wall_->destroy();
        safe_delete( fail_wall_ );
    }

    // プレイヤー開放
    if( player_ != nullptr )
    {
        player_->destroy();
        safe_delete( player_ );
    }

    // ステージ開放
    if( stage_ != nullptr )
    {
        stage_->destroy();
        safe_delete( stage_ );
    }

    // タスクの登録を解除
    TaskManager::getInstance()->unregisterObject( this );
}
/*===========================================================================*/
// 更新処理
void Progress::update()
{
    stage_->update();
    player_->update();
    fail_wall_->update();

    // ステージ変更時のスクロール処理
    if( isScroll() ) { scroll(); }

    // ステージ変更以外のスクロール処理
    else if( player_->getPosition().y > kPlayerStartLine )
    {
        // スタート位置より下に行ったら
        float over = player_->getPosition().y - kPlayerStartLine;
        offset_ = -over;
    }
    else if( player_->getPosition().y < kPlayerEndLine )
    {
        // ゴール位置より上に行ったら
        float over = player_->getPosition().y - kPlayerEndLine;
        offset_ = -over;
    }
    else
    {
        offset_ = 0.0F;
    }
}
/*===========================================================================*/
// 描画処理
void Progress::draw()
{
    stage_->draw( {0.0F, offset_} );
    player_->draw( {0.0F, offset_} );
    fail_wall_->draw( {0.0F, offset_} );
}
/*===========================================================================*/
// 移動
void Progress::setMove( const float Dist )
{
    player_->setMove( Dist );
}


// public
/*===========================================================================*/
// スクロール用初期化処理
void Progress::changeStage()
{
    scroll_sin_ = kScrollSinMinDeg;
    player_scroll_count_ = last_player_scroll_count_ = 0.0F;
    stage_scroll_count_  = last_stage_scroll_count_  = 0.0F;
}


// スクロール関係
/*===========================================================================*/
// スクロールが必要か
bool Progress::isScroll() const 
{
    // スクロール中かどうか
    return 
        scroll_sin_ >= kScrollSinMinDeg &&
        scroll_sin_ <= kScrollSinMaxDeg;
}
// スクロール処理
void Progress::scroll()
{
    // サインカーブ用のサイン値を更新
    scroll_sin_ += kScrollSinChange;
    if( scroll_sin_ > kScrollSinMaxDeg ) { scroll_sin_ = kScrollSinMaxDeg; }


    float percentage = std::sin( toRadians(scroll_sin_) );


    // プレイヤー
    Vector2 player_move;
            player_move.x = 0.0F;
            player_move.y = (kPlayerMotionRange * percentage) - last_player_scroll_count_;
    if( (player_move.y + last_player_scroll_count_) > kPlayerMotionRange )
    {   // 必要以上に移動しないよう制御
        player_move.y = kPlayerMotionRange - last_player_scroll_count_;
    }
    player_->move( player_move );
    player_scroll_count_ += player_move.y;

    // ステージ
    float stage_move  = (kStageMotionrange * percentage) - last_stage_scroll_count_;
    const Vector2& stage_posi = stage_->getPosition();
    stage_->setPosition( {stage_posi.x, stage_posi.y + stage_move} );
    stage_scroll_count_ += stage_move;


    // 終了処理
    last_player_scroll_count_ = player_scroll_count_;
    last_stage_scroll_count_  = stage_scroll_count_;


    // スクロール終了
    if( scroll_sin_ >= kScrollSinMaxDeg ) { scroll_sin_ = -1.0F; }
}
