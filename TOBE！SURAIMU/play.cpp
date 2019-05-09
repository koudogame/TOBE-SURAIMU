
#include "play.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"
#include "task_manager.h"
#include "star_container.h"
#include "result.h"

using PadState = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;

const RECT kTrimmingEffect{
	0L, 0L, 1280L, 720L
};
const RECT kTrimmingBackground{
	0L, 720L, 1280L, 1440L
};

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
	if (do_create_ && create() == false)
	{
		return false;
	}

	// スターの生成
	setStarPattern();
	createStar();

	// 更新関数をstartに
	update_ = &Play::start;



	return true;
}
// 生成処理
bool Play::create()
{
	do_create_ = false;

	// 画像の読み込み
	texture_ = TextureLoder::getInstance()->load(L"Texture/プレイ画面.png");

	// タスクマネージャー
	task_manager_ = new (std::nothrow) TaskManager();
	if (task_manager_ == nullptr) return false;

	// 星のコンテナ
	star_container_ = new (std::nothrow) StarContainer(task_manager_);
	if (star_container_ == nullptr) return false;

	return true;
}

/*===========================================================================*/
// 終了処理
void Play::destroy()
{
	do_create_ = true;

	TextureLoder::getInstance()->release(texture_);

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
	Sprite::getInstance()->draw(
		texture_, Vector2::Zero, &kTrimmingBackground);
	Sprite::getInstance()->draw(
		texture_, Vector2::Zero, &kTrimmingEffect
	);

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
		Pad::getInstance()->getTracker().menu == PadTracker::PRESSED)
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
	if (Key::getInstance()->getTracker().pressed.Enter ||
		Pad::getInstance()->getTracker().b == PadTracker::PRESSED)
	{
		return new Result;
	}

	if (Key::getInstance()->getTracker().pressed.P ||
		Pad::getInstance()->getTracker().menu == PadTracker::PRESSED)
	{
		update_ = &Play::play;
	}


	return this;
}


/*===========================================================================*/
// 星の生成パターンを選択
bool Play::setStarPattern()
{

}
// 星の生成
bool Play::createStar()
{
	Vector2 position;
	float angle;
	float fall_speed;
	float spin_speed;
	float spin_rate;
	float size;

	while (true)
	{
		if (fscanf_s(star_pattern_file_,
			"%f %f %f %f %f %f %f",
			&position.x, &position.y,
			&angle,
			&fall_speed,
			&spin_speed,
			&spin_rate,
			&size) == EOF)
		{
			return true;
		}

		star_container_->addStar(
			position, angle, fall_speed, spin_speed, spin_rate, size);
	}
}
