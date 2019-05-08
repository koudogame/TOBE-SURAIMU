#include "game.h"
#include "release.h"

//-----------------------
//最初のシーン
#include "play.h"

//コンストラクタ
Game::Game()
{
	scene_ = nullptr;
}

//デストラクタ
Game::~Game()
{
	safe_delete( scene_ );
}

//初期化
bool Game::init()
{
	scene_ = new Play;

	if( scene_ == nullptr )
		return false;

	return true;
}

//更新
bool Game::update()
{
	SceneBase* temp = scene_->update();

	if( temp == nullptr )
	{
		return false;
	}

	if( scene_ != temp )
	{
		scene_->destroy();
		temp->init();
		safe_delete( scene_ );
		scene_ = temp;
	}


	return true;
}

//描画
void Game::draw()
{
	scene_->draw();
}

//破棄
void Game::destroy()
{
	scene_->destroy();
}
