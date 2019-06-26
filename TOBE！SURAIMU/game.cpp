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
constexpr int kBackgroundLayerNum = 3;      // 背景レイヤー数
constexpr long kBackgroundSize = 1024L;     // 背景縦横サイズ
constexpr RECT kTrimmingBackground{         // 背景切り取り範囲
	0L, 0L, 1024L, 1024L };
constexpr RECT kTrimmingEffect{             // 背景エフェクト切り取り範囲
	0L, 1024L, 1280L, 1744L };

constexpr float kBackgroundSpeed[]     = { 0.25F, 0.4F, 1.0F };
constexpr float kBackgroundDrawDepth[] = { 0.03F, 0.01F, 0.02F };
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

	texture_ = TextureLoder::getInstance()->load( L"Texture/Background.png" );

	background_container_ = std::make_unique<BackgroundContainer>();
    backobject_container_ = std::make_unique<BackObjectContainer>();

	// 背景の追加
	RECT trimming = kTrimmingBackground;
	for( int i = 0; i < kBackgroundLayerNum; ++i )
	{
		if( background_container_.get()->addBackground(
			trimming ,
			kBackgroundSpeed[ i ] ,
			kBackgroundDrawDepth[ i ] ) == false )
		{
			return false;
		}
		trimming.left += kBackgroundSize;
		trimming.right += kBackgroundSize;
	}

	RECT trim;
	trim.bottom = 1024;
	trim.right = 4096;
	trim.left = trim.right - 1024;
	trim.top = trim.bottom - 1024;
	background_container_.get()->addBackground( trim , 20.0F );

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
	// 背景エフェクト
	Sprite::getInstance()->draw( texture_ , Vector2::Zero , &kTrimmingEffect , 1.0F , 0.09F );

	scene_->draw();
}

//破棄
void Game::destroy()
{
	background_container_.get()->destroy();
    backobject_container_.get()->destroy();
	scene_->destroy();
}
