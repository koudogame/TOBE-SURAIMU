
#include "play.h"

#include "release.h"
#include "key.h"
#include "pad.h"
#include "task_manager.h"
#include "star_container.h"

using ButtonState = GamePad::ButtonStateTracker;


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
// オブジェクトの生成
bool Play::create()
{
	is_create_ = false;

	// タスクマネージャーを生成
	task_manager_ = new (std::nothrow) TaskManager();
	if (task_manager_ == nullptr) return false;

	// 星のコンテナを生成
	star_container_ = new (std::nothrow) StarContainer(task_manager_);
	if (star_container_ == nullptr) return false;

	return true;
}

/*===========================================================================*/
// 終了処理
void Play::destroy()
{
	star_container_->destroy();
	safe_delete(star_container_);

	safe_delete(task_manager_);
}

/*===========================================================================*/
// 更新処理
SceneBase* Play::update()
{
	return (this->*update_)();
}

/*===========================================================================*/
// 描画処理
void Play::draw()
{
	task_manager_->allExecuteDraw();
}


/*===========================================================================*/
// スタート
SceneBase* Play::start()
{
	update_ = &Play::play;
	return this;
}

/*===========================================================================*/
// プレイ
SceneBase* Play::play()
{
	if (Key::getInstance()->getTracker().pressed.P ||
		Pad::getInstance()->getTracker().menu == ButtonState::PRESSED)
	{
		update_ = &Play::pause;
	}
	else
	{
		task_manager_->allExecuteUpdate();
	}
	return this;
}

/*===========================================================================*/
// ポーズ
SceneBase* Play::pause()
{
	if (Key::getInstance()->getTracker().pressed.P ||
		Pad::getInstance()->getTracker().menu == ButtonState::PRESSED)
	{
		update_ = &Play::play;
	}
	return this;
}
