#include "game.h"
#include "release.h"
#include "task_manager.h"
#include "sprite.h"
#include "textureLoder.h"
#include "ranking_manager.h"

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
	0L, 3184L, 1280L, 3904L };

constexpr float kBackgroundSpeed[]     = { 0.2F, 0.4F, 1.0F, 0.6F, };
constexpr float kBackgroundDrawDepth[] = { 0.0F, 0.01F, 0.03F, 0.02F, };
constexpr float kBackobjectMoveX = -1.0F;
constexpr float kBackobjectMoveY = 0.1F;
constexpr float kBackobjectDrawDepth   =  0.05F;
constexpr unsigned kBackobjectKind = 1U;
constexpr RECT kTrimmingBackObject[]   = {
	{ 0L, 0L, 4096L, 1000L},
	{ 800L, 0L, 4096L, 1800L},
	{ 1800L, 0L, 4096L, 2800L},
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

    backobject_container_->addBackObject( 
        kTrimmingBackObject[rand() % kBackobjectKind], 
        kBackobjectMoveX, kBackobjectMoveY, kBackobjectDrawDepth );

	return true;
}

//更新
bool Game::update()
{
	TaskManager::getInstance()->allUpdate();
	//オブジェクトの更新
    if( backobject_container_->empty() )
    {
        backobject_container_->addBackObject(
            kTrimmingBackObject[0],
            kBackobjectMoveX, kBackobjectMoveY, kBackobjectDrawDepth);

    }
    if( backobject_container_->active().size() <= 5 )
    {
        backobject_container_->addBackObject(
            kTrimmingBackObject[0],
            kBackobjectMoveX, kBackobjectMoveY, kBackobjectDrawDepth);
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
	scene_->destroy();
}
