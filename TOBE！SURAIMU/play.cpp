
#include "play.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"
#include "result.h"
#include "task_manager.h"
#include "star_container.h"
#include "player.h"
#include "star.h"

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
	if (do_create_ && create() == false) { return false; }


	// スター生成パターンファイルをリスト化
	FILE* file_name_list;
	errno_t error = fopen_s(&file_name_list, "State/pattern_list.txt", "r");
	if (error != 0) { return false; }
	char file_name[FILENAME_MAX];
	while (fscanf_s(file_name_list, "%s", &file_name, FILENAME_MAX) != EOF)
	{
		star_pattern_list_.push_back(file_name);
	}
	fclose(file_name_list);
	// スターを生成
	if (createStar() == false) { return false; }


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

	// プレイヤー
	player_ = new (std::nothrow) Player(task_manager_);
	if (player_ == nullptr || player_->init(Vector2{ 640.0F,360.0F }) == false)
		return false;

	return true;
}

/*===========================================================================*/
// 終了処理
void Play::destroy()
{
	do_create_ = true;

	TextureLoder::getInstance()->release(texture_);

	safe_delete(player_);

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
	if (Key::getInstance()->getTracker().pressed.Enter ||
		Pad::getInstance()->getTracker().b == PadTracker::PRESSED)
	{
		update_ = &Play::play;
		star_container_->setFall();
	}

	task_manager_->allExecuteUpdate();

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
		star_container_->update();

		// 画面外にあるスターが無くなったらスターを新規生成
		const std::list<Star*> kStarList = star_container_->active();
		auto itr = kStarList.begin();
		auto end = kStarList.end();
		for (; itr != end; ++itr)
		{
			if ((*itr)->getposition().y < 0.0F)
				break;
		}
		if (itr == end && createStar() == false)
		{
			// 生成に失敗
			return nullptr;
		}
		star_container_->setFall();

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
// 星の生成
bool Play::createStar()
{
	// 生成パターンファイルを選択
	errno_t error = fopen_s(
		&star_pattern_, star_pattern_list_[0].c_str(), "r");
	if (error != 0) { return false; }


	// 星の生成
	Vector2 position;
	float angle;
	float fall_speed;
	float spin_speed;
	float spin_rate;
	float size;
	while (true)
	{
		if (fscanf_s(star_pattern_,
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

		position.y -= getWindowHeight<float>();
		star_container_->addStar(
			position, angle, fall_speed, spin_speed, spin_rate, size);
	}

	fclose(star_pattern_);

	return true;
}
