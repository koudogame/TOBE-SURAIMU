
#include "play.h"

#include "release.h"
#include "star_container.h"


/*===========================================================================*/
Play::Play() :
	SceneBase(SceneID::kPlay)
{
}
Play::~Play()
{
}

/*===========================================================================*/
// 初期化処理
bool Play::init()
{
	// 必要オブジェクトの生成
	if (is_create_ && create() == false)
	{
		return false;
	}

	// 更新関数をstartに
	update_ = &Play::start;



	return true;
}

/*===========================================================================*/
// 終了処理
void Play::destroy()
{
	star_->destroy();
}

/*===========================================================================*/
// 更新処理
SceneBase* Play::update()
{
	(this->*update_)();
	return this;
}

/*===========================================================================*/
// 描画処理
void Play::draw()
{

}



/*===========================================================================*/
// オブジェクトの生成
bool Play::create()
{
	is_create_ = false;

	// 星のコンテナを生成
	star_ = new (std::nothrow) StarContainer;
	if (star_ == nullptr) return false;

	return true;
}

/*===========================================================================*/
// スタート
bool Play::start()
{
	update_ = &Play::play;
	return true;
}

/*===========================================================================*/
// プレイ
bool Play::play()
{
	return true;
}

/*===========================================================================*/
// ポーズ
bool Play::pause()
{
	return true;
}
