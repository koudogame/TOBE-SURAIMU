#include "game.h"
#include "release.h"
#include "task_manager.h"

//-----------------------
//最初のシーン
#include "title.h"
//#include "play.h"

//背景処理利用定数
constexpr int kBackgroundLayerNum = 3;      // 背景レイヤー数
constexpr long kBackgroundSize = 1024L;     // 背景縦横サイズ
constexpr RECT kTrimmingBackground{         // 背景切り取り範囲
	0L, 0L, 1024L, 1024L };

constexpr float kBackgroundSpeed[] = { 0.2F, 0.4F, 1.0F, 0.6F, };
constexpr float kBackgroundDrawDepth[] = { 0.0F, 0.1F, 0.3F, 0.2F, };
constexpr RECT kTrimmingBackObject[] = {
	{ 0L, 0L, 1024L, 1024L},
	{ 0L, 0L, 1024L, 1024L},
};
const wchar_t* kBackObjectTexture[] = {
	{L"Texture/roop1.png"},
	{L"Texture/roop2.png"},
	{L"Texture/roop3.png"},
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
	scene_ = std::make_unique<Title>();

	if (scene_.get()->init() == false)
		return false;

	background_container_ = std::make_unique<BackgroundContainer>();
	back_object_container_ = std::make_unique<BackObjectContainer>();

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
	for( int i = 0; i < 3; ++i )
	{
		if( back_object_container_.get()->addBackObject(
			{ 0,0,2048L, 1024L } ,
			-i , 1 , i / 10.0F
		) == false )
		{
			return false;
		}
	}


	return true;
}

//更新
bool Game::update()
{
	TaskManager::getInstance()->allUpdate();

	// 背景オブジェクトが死んでいたら初期化
	if( back_object_container_->empty() )
	{
		for( int i = 0; i < 3; ++i )
		{
			back_object_container_->addBackObject(
				{ 0,0,2048L, 1024L } ,
				-i , 1 , i / 10.0F
			);
		}
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
	scene_->draw();
}

//破棄
void Game::destroy()
{
	background_container_.get()->destroy();
	back_object_container_.get()->destroy();
	scene_->destroy();
}
