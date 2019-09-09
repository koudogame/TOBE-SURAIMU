#include "game.h"
#include "release.h"
#include "task_manager.h"
#include "sprite.h"
#include "textureLoder.h"
#include "ranking_manager.h"
#include "collision.h"
#include "timer.h"
#include "pad.h"

//-----------------------
//最初のシーン
#include "title.h"
#include "endless.h"
//#include "play.h"

//背景処理利用定数
constexpr RECT kTrimmingEffect{             // 背景エフェクト切り取り範囲
	0L, 0L, 1280L, 720L };
constexpr float kBackEffectDepth = 4.0F;

constexpr int   kBackobjectCreateRate = 500;
constexpr int   kBackobjectMax        = 10;
constexpr float kBackobjectMoveXMax   = 0.2F;
constexpr float kBackobjectMoveYMax   = 0.2F;
constexpr float kBackobjectDrawDepth  =  0.05F;
constexpr unsigned kBackobjectKind    = 3U;
constexpr RECT kTrimmingBackObject[]  = {
	{ 0L,    0L, 4096L, 1200L},
	{ 0L, 1200L, 4096L, 2400L},
	{ 0L, 2400L, 4096L, 3600L},
};

const int kEndTime = 3;

//コンストラクタ
Game::Game()
{
	scene_ = nullptr;
}

//デストラクタ
Game::~Game()
{}

//初期化
bool Game::init()
{
    RankingManager::getInstance();

	texture_ = TextureLoder::getInstance()->load(L"Texture/shadow1.png");

	scene_ = std::make_unique<Endless>();

	if (scene_.get()->init() == false)
		return false;
	if (!Background::getInstance()->init())
		return false;

	return true;
}

//更新
bool Game::update()
{
	TaskManager::getInstance()->allUpdate();


	SceneBase* temp = scene_->update();

	if( gameEnd() || temp == nullptr )
	{
		return false;
	}

	if( scene_.get() != temp )
	{
		scene_->destroy();
		temp->init();
		Collision::getInstance()->init();
		scene_.reset( temp );
	}

	return true;
}

//描画
void Game::draw()
{
	TaskManager::getInstance()->allDraw();
	scene_->draw();
    Sprite::getInstance()->reserveDraw(
        texture_,
        Vector2::Zero,
        kTrimmingEffect,
        1.0F,
        kBackEffectDepth
    );
}

//破棄
void Game::destroy()
{
	Background::getInstance()->destroy();
	scene_->destroy();
}

bool Game::gameEnd()
{
	auto state = Pad::getInstance()->getTracker();
	if (state.view == state.PRESSED)
		time_.start();
	if (state.view == state.HELD)
	{
		if ((kEndTime - time_.getCount()) == 0)
			return true;
	}
	return false;
}
