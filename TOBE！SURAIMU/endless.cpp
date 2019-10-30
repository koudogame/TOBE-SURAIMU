
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
#include "stage_data.h"
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
const wchar_t* kStageListFile[] =
{
    L"State/Stage/Easy/stage_list.csv",
    L"State/Stage/Normal/stage_list.csv",
    L"State/Stage/Hard/stage_list.csv",
};
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
constexpr int kStageNum = 3;


// 難易度に関係
constexpr int      kStartStageID = 0;
constexpr int      kStageIDMin = 1;
constexpr int      kStageIDMax = 4;
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
constexpr float kScrollLineTop    = kWindowHeight * 0.3F;
constexpr float kScrollLineBottom = kWindowHeight * 0.85F;


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
    player_         = new Player();
    wall_           = new Wall();
    fail_wall_      = new FailWall();
    progress_       = new Progress();

    // ポーズ初期化
    if( pause_->init() == false ) { return false; }

    // ランキング初期化
    if( ranking_->init() == false ) { return false; }

    // ステージ初期化
    Stage* stage = new Stage();
    decideStagePattern();
    StageData stage_data;
    if( stage_data.load(stage_list_[0]) == false )
        return false;
    if( stage->init( stage_data, player_ ) == false )
        return false;
    stack_stages_.push_front( stage );

	// プレイヤー初期化
    Vector2 player_position = stage_data.player_position;
	if (dynamic_cast<Player*>(player_)->init(player_position, 0) == false)
	{
		return false;
	}


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
    load_stage_idx_ = 1;
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
            for( auto& stage : stack_stages_ )
                stage->start();
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

    scroll();

    if( stack_stages_.front()->getGoalLine() >= -150.0F )
    {
        if( loadNextStage() == false )
            return nullptr;
    }
    if( stack_stages_.back()->getGoalLine() >= fail_wall_->getPosition().y )
    {
        stack_stages_.back()->destroy();
        delete stack_stages_.back();
        stack_stages_.pop_back();
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

/*===========================================================================*/
// スクロール処理
void Endless::scroll()
{
    // プレイヤーの座標
    const Vector2& player_position = player_->getPosition();


    // スクロール範囲に入った分の値を求める
    float over = 0.0F;
    if( player_position.y < kScrollLineTop )            // 上へオーバー
    {
        over = player_position.y - kScrollLineTop;
    }
    else if( player_position.y > kScrollLineBottom )    // 下へオーバー
    {
        over = player_position.y - kScrollLineBottom;
    }


    // タスクマネージャーに登録している全オブジェクトに移動処理を実行させる
    TaskManager::getInstance()->allSetOver( -over );
}

/*===========================================================================*/
// ステージパターンの決定
void Endless::decideStagePattern()
{
    // 第一引数のファイルから、ステージのファイル名を読み込んで
    // 第二引数に選択したファイル名を格納する
    auto decide = [](const std::wstring& FileName, std::wstring* const pWstring)
    {
        CsvLoader data( FileName );

        int stage_id = rand() % data.getNumber( 0, 0 ) + 1;

        *pWstring = data.getString( 0, stage_id );
    };

    for( int i = 0; i < kStageNum; ++i )
        decide( kStageListFile[i], &stage_list_[i] );
}

/*===========================================================================*/
// ネクストステージの読み込み
bool Endless::loadNextStage()
{
    // 新規ステージの作成&読み込み
    Stage* new_stage = new Stage();
    if( new_stage->init( 
            stage_list_[load_stage_idx_], 
            player_, 
            stack_stages_.front()->getGoalLine() 
        ) == false )
    {
        delete new_stage;
        return false;
    }

    // ステージを追加
    new_stage->start();
    stack_stages_.push_front( new_stage );

    // ロードステージ番号が範囲外へ行かないよう制御
    if( ++load_stage_idx_ >= kStageNum )
        load_stage_idx_ = 0;

    return true;
}
