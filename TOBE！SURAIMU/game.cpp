#include "game.h"
#include "release.h"
#include "task_manager.h"
#include "sprite.h"
#include "textureLoder.h"
#include "ranking_manager.h"
#include "collision.h"

//-----------------------
//最初のシーン
#include "title.h"
//#include "play.h"

//背景処理利用定数
constexpr RECT kTrimmingEffect{             // 背景エフェクト切り取り範囲
	0L, 1024L, 1280L, 1744L };
constexpr float kBackEffectDepth = 4.0F;

constexpr int   kBackobjectCreateRate  = 500;
constexpr int   kBackobjectMax         = 10;
constexpr float kBackobjectMoveXMax    = 0.2F;
constexpr float kBackobjectMoveYMax    = 0.2F;
constexpr float kBackobjectDrawDepth   =  0.05F;
constexpr unsigned kBackobjectKind     = 3U;
constexpr RECT kTrimmingBackObject[]   = {
	{ 0L,    0L, 4096L, 1200L},
	{ 0L, 1200L, 4096L, 2400L},
	{ 0L, 2400L, 4096L, 3600L},
};

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

	scene_ = std::make_unique<Title>();

	if (scene_.get()->init() == false)
		return false;
	if (!background_.init())
		return false;

    backobject_container_ = std::make_unique<BackObjectContainer>();

    backobject_container_->addBackObject(
        kTrimmingBackObject[rand() % kBackobjectKind],
        (rand() % static_cast<int>(kBackobjectMoveXMax * 10.0F)) / 10.0F + 0.1F * (rand() % 2 ? 1 : -1),
        (rand() % static_cast<int>(kBackobjectMoveYMax * 10.0F)) / 10.0F + 0.1F,
        kBackobjectDrawDepth);

	return true;
}

//更新
bool Game::update()
{
	TaskManager::getInstance()->allUpdate();
	//オブジェクトの更新
    backobject_container_->update();
    if( backobject_container_->empty() ||
        (!(rand() % kBackobjectCreateRate) &&
         backobject_container_->active().size() < kBackobjectMax) )
    {
        backobject_container_->addBackObject(
            kTrimmingBackObject[rand() % kBackobjectKind],
            (rand() % static_cast<int>(kBackobjectMoveXMax * 10.0F)) / 10.0F + 0.1F * (rand() % 2 ? 1 : -1),
            (rand() % static_cast<int>(kBackobjectMoveYMax * 10.0F)) / 10.0F + 0.1F,
            kBackobjectDrawDepth);
    }


	SceneBase* temp = scene_->update();

	if( temp == nullptr )
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
	background_.destroy();
    backobject_container_.get()->destroy();
	scene_->destroy();
}
