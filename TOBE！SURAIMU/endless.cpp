
// 板場 温樹

#include "endless.h"

#include <direct.h>
#include "release.h"
#include "key.h"
#include "pad.h"
#include "textureLoder.h"
#include "sprite.h"
#include "csvLoader.h"


#include "text.h"
#include "timer.h"
#include "space.h"
#include "task_manager.h"

#include "pause.h"
#include "ranking_in_endless.h"
#include "progress.h"
#include "background.h"
#include "stage.h"
#include "player.h"
#include "wall.h"
#include "fail_wall.h"

#include "title.h"
#include "endless.h"
#include "result.h"

#include "Sound.h"


#include "ai_demo.h"

using KeyTracker = Keyboard::KeyboardStateTracker;
using PadTracker = GamePad::ButtonStateTracker;

/*===========================================================================*/
// 処理に関係
constexpr wchar_t kStartStageFile[] = L"State/start_pattern.csv";
constexpr float kWindowWidth  = getWindowWidth<float>();
constexpr float kWindowHeight = getWindowHeight<float>();
constexpr float kStageHeight = 7920.0F;
enum { kAButton, kStick };
constexpr Vector2 kPosition = Vector2(kWindowWidth - 272.0F, kWindowHeight - 128.0F);
const RECT kTrimming[] = {
    { 256L, 0L, 512L, 128L },
    {   0L, 0L, 256L, 128L }
};
constexpr float kDispTimeMSPF = 500.0F / 16.0F;                 // 難易度上昇時、スクロールにかける時間( ミリ秒/フレーム )


// 難易度に関係
constexpr int      kStartStageID = 0;
constexpr int      kStageIDMin = 1;
constexpr int      kStageIDMax = 4;
constexpr int      kStageNum = 3;                               // ステージ数
constexpr float    kFailWallUpStartLine = 1000.0F;              // 炎が上昇を開始する位置( この距離分プレイヤーが上昇したら、炎の上昇スタート )
constexpr unsigned kHeight = 0U;                                // レベルテーブル : ステージの長さ( 高さ )
constexpr unsigned kThresholdUp = 1U;                           // レベルテーブル : 閾値( スクロール↑ )
constexpr unsigned kThresholdDown = 2U;                         // レベルテーブル : 閾値( スクロール↓ )
constexpr float kLevelTable[][3] = {                            // レベルテーブル
    {   3000.0F, kWindowHeight * 0.3F, kWindowHeight * 0.85F },
    {   3000.0F, kWindowHeight * 0.3F, kWindowHeight * 0.85F },
    {   3000.0F, kWindowHeight * 0.3F, kWindowHeight * 0.85F },
    {   7500.0f, kWindowHeight * 0.3F, kWindowHeight * 0.85F },
    {  10000.0F, kWindowHeight * 0.3F, kWindowHeight * 0.85F }
};



constexpr Vector2 kPlayerPosition { 600.0F, 565.0F };


/*===========================================================================*/
Endless::Endless()
{
}

Endless::~Endless()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool Endless::init()
{
    destroy();
    created_ = true;


    clock_          = new Timer<Milliseconds>();

    pause_          = new Pause();

    ranking_        = new RankingInEndless();

    now_stage_      = new Stage;

    player_         = new Player();

    wall_           = new Wall();

    fail_wall_      = new FailWall();

    progress_       = new Progress();


    // ポーズ初期化
    if( pause_->init() == false ) { return false; }

    // ランキング初期化
    if( ranking_->init() == false ) { return false; }


	// プレイヤー初期化
	if (dynamic_cast<Player*>(player_)->init(kPlayerPosition, 0) == false)
	{
		return false;
	}

    // ステージ初期化
    if( now_stage_->init( L"State/StageMode/stage_2.csv", player_ ) == false )
        return false;


	// 壁初期化
	if (wall_->init() == false) { return false; }
    if( fail_wall_->init() == false ) { return false; }
    Background::getInstance()->setFailWall( fail_wall_ );

    // 進行度初期化
    if( progress_->init( kStageHeight, player_, fail_wall_ ) == false )
    {
        return false;
    }


	// 変数初期化
	update_ = &Endless::start;
    stage_ = 0;
    round_counter_ = 0;
    is_pause_ = false;
    scroll_threshold_ = kLevelTable[0U][kThresholdUp];
    offset_ = 0.0F;
    offset_one_frame_ = 0.0F;
	climb_ = 0.0F;
    player_last_position_y_ = player_->getPosition().y;
    player_displacement_y_sum_ = 0.0F;

	clock_->start();

	//サウンドの再生
	SOUND->stop( SoundId::kPlay );
	SOUND->play( SoundId::kPlay , true );

	return true;
}
/*===========================================================================*/
// 終了処理
void Endless::destroy()
{
     if( created_ == false ) { return; }
    created_ = false;

    progress_->destroy();              safe_delete(progress_);

    fail_wall_->destroy();             safe_delete(fail_wall_);
    Background::getInstance()->setFailWall( nullptr );

	wall_->destroy();                  safe_delete(wall_);

    for( auto& stage : stack_stages_ )
    {
        stage->destroy();
        safe_delete( stage );
    }
    stack_stages_.clear();
    now_stage_->destroy();             safe_delete( now_stage_ );
    
	player_->destroy();                safe_delete(player_);

    ranking_->destroy();               safe_delete(ranking_);

    pause_->destroy();                 safe_delete(pause_);

	safe_delete(clock_);
}

/*===========================================================================*/
// 更新関数
SceneBase* Endless::update()
{
	SceneBase* scene = (this->*update_)();

    // 衝突処理
    Space::getInstance()->collision();

    return scene;
}

/*===========================================================================*/
// 描画関数
void Endless::draw()
{
    // ポーズ中だったらポーズを描画
    if( update_ == &Endless::pause )
    {
        pause_->draw();
    }
}

/*===========================================================================*/
// スタート部
SceneBase* Endless::start()
{
    KeyTracker key = Key::getInstance()->getTracker();
    PadTracker pad = Pad::getInstance()->getTracker();

    // ポーズ画面へ
    if( key.pressed.P || pad.start == PadTracker::PRESSED )
    {
		SOUND->stop( SoundId::kDicision );
		SOUND->play( SoundId::kDicision , false );
        is_pause_ = true;
		player_->getScore()->stop();
        pause_->reset();
        TaskManager::getInstance()->pause();
        update_ = &Endless::pause;
    }
	else
	{
	    // プレイヤーがジャンプしたらplay部へ
        if (player_->isJump())
        {
		    update_ = &Endless::play;
		    clock_->start();
		    player_->onStartFlag();
            fail_wall_->start();
        }
	}

	return this;
}
// プレイ部
SceneBase* Endless::play()
{
    KeyTracker key = Key::getInstance()->getTracker();
    PadTracker pad = Pad::getInstance()->getTracker();
    if( key.pressed.P || pad.start == PadTracker::PRESSED )
    {
		SOUND->stop( SoundId::kDicision );
		SOUND->play( SoundId::kDicision , false );
		player_->getScore()->timeStop();
        clock_->stop();
        pause_->reset();
        TaskManager::getInstance()->pause();
        update_ = &Endless::pause;
        return this;
    }


	// プレイヤーが死んでいたらリザルト画面へ
	if( player_->isAlive() == false )
	{
		SOUND->stop( SoundId::kPlay );
		return new Result(ranking_->getRank(), *player_->getScore());
        //return new Endless();
	}
    else if( fail_wall_->isUp() == false )
    {
        // プレイヤーが一定の量上へ進んだら炎の上昇をスタートする
        const Vector2& p_position = player_->getPosition();
        player_displacement_y_sum_ += player_last_position_y_ - p_position.y;

        if( player_displacement_y_sum_ >= kFailWallUpStartLine )
        {
            fail_wall_->upStart();
        }
        player_last_position_y_ = p_position.y;
    }


	ranking_->setScore( player_->getScore()->getScore() );


	return this;
}
// ポーズ部
SceneBase* Endless::pause()
{
    TaskManager* const kTaskManager = TaskManager::getInstance();

    KeyTracker key = Key::getInstance()->getTracker();
    PadTracker pad = Pad::getInstance()->getTracker();


    // ポーズからの戻り値で処理を分岐
    switch( pause_->update() )
    {
    case Pause::kContinue :
        kTaskManager->restart();
        clock_->restart();
		player_->getScore()->timeRestart();
        update_ = is_pause_ ? &Endless::start : &Endless::play;
        is_pause_ = false;
        break;

    case Pause::kRestart  :
		player_->getScore()->timeRestart();
        kTaskManager->restart();
        Background::getInstance()->reset();
        return new Endless();

    case Pause::kTitle    :
        kTaskManager->restart();
		SOUND->stop( SoundId::kPlay );
        return new Title();
    }


    return this;
}
