
// 板場 温樹

#include "endless.h"

#include <direct.h>
#include "release.h"
#include "key.h"
#include "pad.h"
#include "textureLoder.h"
#include "sprite.h"
#include "csvLoader.h"

#include "ranking_in_endless.h"
#include "timer.h"
#include "collision.h"
#include "task_manager.h"
#include "background_container.h"
#include "back_object_container.h"
#include "star_container.h"
#include "player.h"
#include "wall.h"

#include "result.h"
#include "text.h"

using PadState = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;

/*===========================================================================*/
// 難易度に関係
constexpr Vector2 kInitStarPosi[]   = {
    {640.0F, 600.0F},
    {810.0F, 100.0F},
};
constexpr float kInitStarAngle[]    = { 90.0F, 90.0F, };
constexpr float kInitStarFall[]     = { 2.0F,1.0F, };
constexpr float kInitStarSpin[]     = { 3.0F,3.0F, };
constexpr float kInitStarSpinRate[] = { 0.001F,0.01F, };
constexpr float kInitStarSize[]     = { 90.0F,120.0F, };

constexpr float kThresholdY = 360.0F;       // スクロール閾値


/*===========================================================================*/
Endless::Endless()
{
}

Endless::~Endless()
{
	// 開放忘れ対応
	if (do_create_ == false)
	{
		destroy();
	}
}

/*===========================================================================*/
// 初期化処理
bool Endless::init()
{
	// 生成処理
	if (do_create_ && create() == false) { return false; }

    // ランキング初期化
    ranking_->init();

	// 初期スターの生成
	for (int i = 0; i < 2; ++i)
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
	CsvLoader file(L"State/player_state.csv");
	Vector2 position;
    position.x       = file.getNumber_f(0, 1);
    position.y       = file.getNumber_f(1, 1);
	float jump       = file.getNumber_f(2, 1);
	float add_vol    = file.getNumber_f(3, 1);
	float decay      = file.getNumber_f(4, 1);
	float gravity    = file.getNumber_f(5, 1);
	float speed      = file.getNumber_f(6, 1);
	float rl_boost   = file.getNumber_f(7, 1);
	if (player_->init(
		position,jump,add_vol,decay,gravity,speed,rl_boost) == false)
	{
		return false;
	}

	// 壁初期化
	if (wall_->init() == false) { return false; }



	// 変数初期化
	update_ = &Endless::start;
	climb_ = 0.0F;
	magnification_ = 1.0F;

	clock_->start();

	return true;
}
// 生成処理
bool Endless::create()
{
	if (do_create_ == false) { return true; }
	do_create_ = false;

	// 時計
	clock_                 = new Timer<Milliseconds>();

    // ランキング
    ranking_               = new RankingInEndless();

	// スターコンテナ
	star_container_        = new StarContainer();

	// プレイヤー
	player_                = new Player();

	// 壁
	wall_                  = new Wall();


	// スター生成パターンファイルのリスト化
	std::wstring file_name;
	CsvLoader file(L"State/pattern_list.csv");
	for (int i = 0; ; ++i)
	{
		file_name = file.getString(0, i);
		if (wcscmp(file_name.c_str(), L"") == 0)
		{
			break;
		}

        file_name.insert(0,L"State/");
		star_container_->addPattern(file_name);
	}


	return true;
}

/*===========================================================================*/
// 終了処理
void Endless::destroy()
{
	do_create_ = true;

	// 壁
	wall_->destroy();                  safe_delete(wall_);

	// プレイヤー
	player_->destroy();                safe_delete(player_);

	// スターコンテナ
	star_container_->destroy();        safe_delete(star_container_);

    // ランキング
    ranking_->destroy();               safe_delete(ranking_);

	// 時計
	safe_delete(clock_);
}

/*===========================================================================*/
// 更新関数
SceneBase* Endless::update()
{
	return (this->*update_)();
}

/*===========================================================================*/
// 描画関数
void Endless::draw()
{

}

/*===========================================================================*/
// スタート部
SceneBase* Endless::start()
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
	}

	// 星との衝突処理
	for (auto& star : star_container_->active())
	{
		Collision::getInstance()->collision(player_, star);
	}


	return this;
}
// プレイ部
SceneBase* Endless::play()
{
    // スターを生成する
    checkAndCreateStar();

	// プレイヤーが死んでいたらリザルト画面へ
	if (player_->isAlive() == false)
	{
		return new Result;
	}

	//コンテナのアップデート
	star_container_->update();

	// 座標調整( スクロール )
	const float kOver = kThresholdY - player_->getPosition().y;
	if( kOver > 0.0F ) 
    {
        player_->addScore( kOver );
        climb_ += kOver;
    }
	adjustObjectPosition(kOver);

    ranking_->setScore(player_->getScore());

	// オブジェクトの状態倍率を更新
	const float kMagnification = climb_ / 100000.0F + 1.0F;
	if ((kMagnification - magnification_) > 0.1F)
	{
		magnification_ = kMagnification - magnification_ + 1.0F;
		player_->resetStatus(magnification_);
		star_container_->resetStates(magnification_);
	}

	// 衝突処理
	Collision* const kCollision = Collision::getInstance();
	for (auto& star : star_container_->active())
	{
		kCollision->collision(player_, star);
	}
	kCollision->collision(player_, wall_);

	return this;
}

/*===========================================================================*/
// オブジェクトの座標調整
void Endless::adjustObjectPosition(const float Over)
{
	if (Over > 0)
	{
        TaskManager::getInstance()->allSetOver(Over);
	}
}

/*===========================================================================*/
// スターの生成条件を満たしていたらスターを生成する
bool Endless::checkAndCreateStar()
{
    // 画面外待機しているスターが無くなったらスターの生成
    auto itr = star_container_->active().begin();
    auto end = star_container_->active().end();
    for (; itr != end; ++itr)
    {
        if ((*itr)->getPosition().y < 0.0F) { break; }
    }

    if (itr == end &&
        star_container_->createStar() == false)
    {
        return false;
    }


    return true;
}
