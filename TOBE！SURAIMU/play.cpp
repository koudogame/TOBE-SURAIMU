
#include "play.h"

#include "release.h"
#include "task_manager.h"
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
	(this->*update_)();
	return this;
}

/*===========================================================================*/
// 描画処理
void Play::draw()
{
	task_manager_->allExecuteDraw();
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
	task_manager_->allExecuteUpdate();
	return true;
}

/*===========================================================================*/
// ポーズ
bool Play::pause()
{
	return true;
}
