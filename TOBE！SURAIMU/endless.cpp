
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
#include "pause.h"
#include "ranking_in_endless.h"
#include "space.h"
#include "task_manager.h"
#include "back_object_container.h"
#include "star_container.h"
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
constexpr float kWindowWidth  = getWindowWidth<float>();
constexpr float kWindowHeight = getWindowHeight<float>();

enum { kAButton, kStick };
constexpr Vector2 kPosition = Vector2(kWindowWidth - 272.0F, kWindowHeight - 128.0F);
const RECT kTrimming[] = {
    { 256L, 0L, 512L, 128L },
    {   0L, 0L, 256L, 128L }
};

constexpr float kDispTimeMSPF = 500.0F / 16.0F;                 // 難易度上昇時、スクロールにかける時間( ミリ秒/フレーム )
// 難易度に関係
constexpr int      kStageNum = 3;                               // ステージ数
constexpr unsigned kHeight = 0U;                                // レベルテーブル : ステージの長さ( 高さ )
constexpr unsigned kThresholdUp = 1U;                           // レベルテーブル : 閾値( スクロール↑ )
constexpr unsigned kThresholdDown = 2U;                         // レベルテーブル : 閾値( スクロール↓ )
constexpr float kLevelTable[][3] = {                            // レベルテーブル
    {   3000.0F, kWindowHeight * 0.10F, kWindowHeight * 0.75F },
    {   3000.0F, kWindowHeight * 0.10F, kWindowHeight * 0.75F },
    {   3000.0F, kWindowHeight * 0.10F, kWindowHeight * 0.75F },
    {   7500.0f, kWindowHeight * 0.10F, kWindowHeight * 0.75F },
    {  10000.0F, kWindowHeight * 0.10F, kWindowHeight * 0.75F }
};



constexpr Vector2 kPlayerPosition { 600.0F, 565.0F };

constexpr Vector2 kInitStarPosi[]   = {                         // 初期スター位置
    {640.0F, 600.0F},
    {816.0F, 297.0F},
	{465.0F, 142.0F},
};
constexpr float kInitStarAngle[]    = { 90.0F, 90.0F,90.0F, };  // 初期スター角度
constexpr float kInitStarSpin[]		= { -3.0F, 3.0F,3.0F };     // 初期スター回転速度
constexpr float kInitStarSpinRate[] = { 0.2F, 0.2F,0.2F };      // 初期スター回転割合
constexpr float kInitStarSize[]     = { 90.0F, 120.0F, 120.0F}; // 初期スター大きさ


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

    star_container_ = new StarContainer();

    player_         = new Player();

    wall_           = new Wall();

    fail_wall_      = new FailWall();


    // ポーズ初期化
    if( pause_->init() == false ) { return false; }

    // ランキング初期化
    if( ranking_->init() == false ) { return false; }

	// 初期スターの生成
	for (int i = 0; i < 3; ++i)
	{
		if (star_container_->addStar(
			kInitStarPosi[i],
			kInitStarAngle[i],
			kInitStarSpin[i],
			kInitStarSpinRate[i],
			kInitStarSize[i]
		) == nullptr)
		{
			return false;
		}
	}

	// プレイヤー初期化
	if (dynamic_cast<Player*>(player_)->init(kPlayerPosition, 0) == false)
	{
		return false;
	}

	// 壁初期化
	if (wall_->init() == false) { return false; }

    if( fail_wall_->init() == false ) { return false; }


	// 変数初期化
	update_ = &Endless::start;
    stage_ = 0;
    round_counter_ = 0;
    is_pause_ = false;
    scroll_threshold_ = kLevelTable[0U][kThresholdUp];
    offset_ = 0.0F;
    offset_one_frame_ = 0.0F;
	climb_ = 0.0F;
    changePattern( stage_ );    // スター生成パターン設定

	clock_->start();

	//サウンドの再生
	SOUND->stop( SoundId::kPlay );
	SOUND->play( SoundId::kPlay , true );

	description_ = TextureLoder::getInstance()->load( L"Texture/a_jump.png" );

	return true;
}
/*===========================================================================*/
// 終了処理
void Endless::destroy()
{
     if( created_ == false ) { return; }
    created_ = false;

    fail_wall_->destroy();             safe_delete(fail_wall_);

	wall_->destroy();                  safe_delete(wall_);

	player_->destroy();                safe_delete(player_);

	star_container_->destroy();        safe_delete(star_container_);

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

	if( player_->guide() > 0.0F )
	{
		if (player_->isJump())
		{
			Sprite::getInstance()->reserveDraw(
				description_, kPosition, kTrimming[kStick],
				1.0F, 99.0F);
		}
		else if(!player_->isJump())
		{
			Sprite::getInstance()->reserveDraw(
				description_, kPosition, kTrimming[kAButton],
				1.0F, 99.0F);
		}

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
		    for (auto& star : star_container_->active())
		    {
			    star->setFall();
		    }
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
        clock_->stop();
        pause_->reset();
        TaskManager::getInstance()->pause();
        update_ = &Endless::pause;
        return this;
    }


    // スターを生成する
    checkAndCreateStar();

	// プレイヤーが死んでいたらリザルト画面へ
	if( player_->isAlive() == false )
	{
		SOUND->stop( SoundId::kPlay );
		return new Result(ranking_->getRank(), *player_->getScore());
        //return new Endless();
	}

	//コンテナのアップデート
	star_container_->update();

	// 座標調整( スクロール )
    scroll();



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
        update_ = is_pause_ ? &Endless::start : &Endless::play;
        is_pause_ = false;
        break;

    case Pause::kRestart  :
        kTaskManager->restart();
        return new Endless();

    case Pause::kTitle    :
        kTaskManager->restart();
		SOUND->stop( SoundId::kPlay );
        return new Title();
    }


    return this;
}

/*===========================================================================*/
// 画面スクロール処理
void Endless::scroll()
{
    float over = 0; // プレイアブルエリアからはみ出した距離
    const Vector2& kPlayerPosi = player_->getPosition();

    // 上限からはみ出している
    if( kPlayerPosi.y < kLevelTable[stage_][kThresholdUp] )
    {
        over = kLevelTable[stage_][kThresholdUp] - kPlayerPosi.y;
    }
    // 下限からはみ出している
    else if( kPlayerPosi.y > kLevelTable[stage_][kThresholdDown] )
    {
        over = kLevelTable[stage_][kThresholdDown] - kPlayerPosi.y;
    }


    // 各オブジェクトのスクロール
    TaskManager::getInstance()->allSetOver( over );

    if( over > 0.0F ) 
    {
        climb_ += over;
        player_->addScore( over );  // プレイヤーにスコア反映
    }


    // 現在のステージを突破したら
    if( climb_ >= kLevelTable[stage_][kHeight] )
    {
        climb_ = 0.0F;
        ++stage_;
        if( stage_ >= kStageNum )
        {
            ++round_counter_;
            stage_ = 0;

            // 周回を知らせる
            player_->addLevel();
            fail_wall_->levelUp();
        }

        // スターの生成パターン変更
        changePattern( stage_ );
    }
}


/*===========================================================================*/
// スターの生成条件を満たしていたらスターを生成する
bool Endless::checkAndCreateStar()
{
    auto itr = star_container_->active().begin();
    auto end = star_container_->active().end();
    for (; itr != end; ++itr)
    {
        if ((*itr)->getPosition().y < 0.0F) { break; }
    }

    // 画面外待機しているスターが無くなったらスターの生成
    if (itr == end &&
        star_container_->createStar() == false)
    {
        return false;
    }


    return true;
}

/*===========================================================================*/
// スターの生成パターンを設定する
void Endless::changePattern( const int Pattern )
{
    star_container_->resetPattern();

    std::wstring file_name;
    CsvLoader file(L"State/pattern_list.csv");
    for (int i = 0; ; ++i)
    {
        file_name = file.getString(Pattern, i);
        if (wcscmp(file_name.c_str(), L"") == 0)
        {
            break;
        }

        file_name.insert(0, L"State/");
        star_container_->addPattern(file_name);
    }
}
