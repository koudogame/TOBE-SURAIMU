
// 板場 温樹

#include "endless.h"

#include <direct.h>
#include "release.h"
#include "key.h"
#include "pad.h"
#include "textureLoder.h"
#include "sprite.h"
#include "csvLoader.h"

#include "timer.h"
#include "collision.h"
#include "task_manager.h"
#include "background.h"
#include "back_object.h"
#include "star_container.h"
#include "player.h"
#include "wall.h"
#include "combo.h"

#include "result.h"

using PadState = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;

/*===========================================================================*/
// 難易度に関係
constexpr Vector2 kInitStarPosi[] = {
	{640.0F, 600.0F},
	{810.0F, 100.0F},
};
constexpr float kInitStarAngle[] = {
	90.0F,
	90.0F,
};
constexpr float kInitStarFall[] = {
	2.0F,
	1.0F,
};
constexpr float kInitStarSpin[] = {
	3.0F,
	3.0F,
};
constexpr float kInitStarSpinRate[] = {
	0.01F,
	0.01F,
};
constexpr float kInitStarSize[] = {
	90.0F,
	120.0F
};

// 処理に関係
constexpr long kBackgroundSize = 1024L;     // 背景縦横サイズ
constexpr RECT kTrimmingBackground{         // 背景切り取り範囲
	0L, 0L, 1024L, 1024L };
constexpr RECT kTrimmingEffect{             // 背景エフェクト切り取り範囲
	0L,   0L, 1280L,  720L };
constexpr float kThresholdY = 360.0F;       // プレイヤーの限界Y座標( 絶対座標 )
constexpr float kBackgroundSpeed[] =
{
	0.2F,
	0.4F,
	1.0F,
	0.6F
};
constexpr float kBackgroundDrawDepth[] = 
{
    0.0F,
    0.1F,
    0.3F,
    0.2F,
};
constexpr RECT kTrimmingBackObject[] = 
{
	{0L, 1024L, 1280L, 1744L},
	{1280L, 1024LL, 2560L, 1744L},
};


/*===========================================================================*/
Endless::Endless()
{
}

Endless::~Endless()
{
	// 開放忘れ対応
	if (do_create_ == false)
	{
		destroy();
	}
}

/*===========================================================================*/
// 初期化処理
bool Endless::init()
{
	// 生成処理
	if (do_create_ && create() == false) { return false; }

	// 初期スターの生成
	for (int i = 0; i < 2; ++i)
	{
		if (star_container_->addStar(
			kInitStarPosi[i],
			kInitStarAngle[i],
			kInitStarSpin[i],
			kInitStarSpinRate[i],
			kInitStarSize[i]
		) == nullptr)
		{
			return false;
		}
	}


	// プレイヤー初期化
	CsvLoader file(L"State/player_state.csv");
	Vector2 position = Vector2(file.getNumber_f(0, 1),
							   file.getNumber_f(1, 1));
	float jump       = file.getNumber_f(2, 1);
	float add_vol    = file.getNumber_f(3, 1);
	float decay      = file.getNumber_f(4, 1);
	float gravity    = file.getNumber_f(5, 1);
	float speed      = file.getNumber_f(6, 1);
	float rl_boost   = file.getNumber_f(7, 1);
	if (player_->init(
		position,jump,add_vol,decay,gravity,speed,rl_boost) == false)
	{
		return false;
	}

	// 背景
	RECT trimming = kTrimmingBackground;
	for (int i = 0; i < kBackgroundLayerNum; ++i)
	{
		if (background_[i]->init(
			L"Texture/background.png",
			trimming,
			kBackgroundSpeed[i],
            kBackgroundDrawDepth[i]) == false)
		{
			return false;
		}
		trimming.left += kBackgroundSize;
		trimming.right += kBackgroundSize;
	}
	if(back_object_->init(
		kTrimmingBackObject[rand() % 2],
		kBackgroundSpeed[3],
        kBackgroundDrawDepth[3]) == false)
	{
		return false;
	}


	// 壁初期化
	if (wall_->init() == false) { return false; }

	// UI初期化
	if (combo_->init(*clock_) == false) { return false; }



	// 変数初期化
	update_ = &Endless::start;
	climb_ = 0.0F;
	prev_player_owner_ = nullptr;
	prev_player_jump_state_ = false;
	magnification_ = 1.0F;

	clock_->start();

	return true;
}
// 生成処理
bool Endless::create()
{
	if (do_create_ == false) { return true; }
	do_create_ = false;

	// テクスチャ
	TextureLoder* const kTexture = TextureLoder::getInstance();
	texture_ = kTexture->load(L"Texture/プレイ画面.png");
	if (texture_ == nullptr)        { return false; }
	texture_numbers_ = kTexture->load(L"Texture/数字.png");
	if (texture_numbers_ == nullptr){ return false; }

	// 時計
	clock_              = new (std::nothrow) Timer<Milliseconds>();
	if (clock_ == nullptr)          { return false; }

	// タスクマネ－ジャー
	task_manager_       = new (std::nothrow) TaskManager();
	if (task_manager_ == nullptr)   { return false; }

	// 背景
	for (auto& background : background_)
	{
		background      = new Background(task_manager_);
		if (background == nullptr)  { return false; }
	}
	back_object_        = new (std::nothrow) BackObject(task_manager_);
	if(back_object_ == nullptr)    { return false; }

	// スターコンテナ
	star_container_     = new (std::nothrow) StarContainer(task_manager_);
	if (star_container_ == nullptr) { return false; }

	// プレイヤー
	player_             = new (std::nothrow) Player(task_manager_);
	if (player_ == nullptr)         { return false; }

	// 壁
	wall_               = new (std::nothrow) Wall(task_manager_);
	if (wall_ == nullptr)           { return false; }

	// UI
	combo_              = new (std::nothrow) Combo(task_manager_);
	if (combo_ == nullptr)          { return false; }



	// スター生成パターンファイルのリスト化
	std::wstring file_name;
	CsvLoader file(L"State/pattern_list.csv");
	for (int i = 0; ; ++i)
	{
		file_name = file.getString(0, i);
		if (wcscmp(file_name.c_str(), L"") == 0)
		{
			break;
		}

		pattern_file_.push_back(file_name);
	}


	return true;
}

/*===========================================================================*/
// 終了処理
void Endless::destroy()
{
	do_create_ = true;

	//UI
	combo_->destroy();          safe_delete(combo_);

	// 壁
	wall_->destroy();           safe_delete(wall_);

	// プレイヤー
	player_->destroy();         safe_delete(player_);

	// スターコンテナ
	star_container_->destroy(); safe_delete(star_container_);

	// 背景
	back_object_->destroy();    safe_delete(back_object_);
	for (auto& background : background_)
	{
		background->destroy();  safe_delete(background);
	}

	// タスクマネージャー
	safe_delete(task_manager_);

	// 時計
	safe_delete(clock_);

	// テクスチャ
	TextureLoder* const kTexture = TextureLoder::getInstance();
	kTexture->release(texture_);
	kTexture->release(texture_numbers_);
}

/*===========================================================================*/
// 更新関数
SceneBase* Endless::update()
{
	return (this->*update_)();
}

/*===========================================================================*/
// 描画関数
void Endless::draw()
{
	Sprite* const kSprite = Sprite::getInstance();


	// オブジェクト
	task_manager_->allDraw();


	// 背景エフェクト
	kSprite->draw(texture_, Vector2::Zero, &kTrimmingEffect);

	Numbers<int> climb(static_cast<int>(climb_));
	climb.draw(
		texture_numbers_,
		Vector2(1280.0F, 0.0F),
		64L,
		128L
	);
}

/*===========================================================================*/
// スタート部
SceneBase* Endless::start()
{
	// プレイヤーがジャンプしたらplay部へ
	if (player_->isJump())
	{
		update_ = &Endless::play;
		for (auto& star : star_container_->active())
		{
			star->setFall();
		}
		clock_->start();
	}

	// オブジェクト更新
	task_manager_->allUpdate();
	// 背景オブジェクトが死んでいたら初期化
	if (back_object_->isAlive() == false)
	{
		back_object_->reset(kTrimmingBackObject[rand() % 2]);
	}

	// 星との衝突処理
	for (auto& star : star_container_->active())
	{
		Collision::getInstance()->collision(player_, star);
	}


	return this;
}
// プレイ部
SceneBase* Endless::play()
{
	// 画面外待機している星が無くなったら星の生成
	auto itr = star_container_->active().begin();
	auto end = star_container_->active().end();
	for (; itr != end; ++itr)
	{
		if ((*itr)->getPosition().y < 0.0F) { break; }
	}
	if (itr == end && createStar() == false) { return nullptr; }


	// オブジェクト更新
	task_manager_->allUpdate();
	scoring();

	// プレイヤーが死んでいたらリザルト画面へ
	if (player_->isAlive() == false)
	{
		return new Result;
	}

	// 座標調整( スクロール )
	const float kOver = kThresholdY - player_->getPosition().y;
	if (kOver > 0.0F) { climb_ += kOver; }
	adjustObjectPosition(kOver);
	// 背景オブジェクトが死んでいたら初期化
	if(back_object_->isAlive() == false)
	{
		back_object_->reset( kTrimmingBackObject[rand() % 2] );
	}

	// オブジェクトの状態倍率を更新
	const float kMagnification = climb_ / 100000.0F + 1.0F;
	if ((kMagnification - magnification_) > 0.1F)
	{
		magnification_ = kMagnification - magnification_ + 1.0F;
		player_->resetStatus(magnification_);
		star_container_->resetStates(magnification_);
	}

	// 衝突処理
	Collision* const kCollision = Collision::getInstance();
	for (auto& star : star_container_->active())
	{
		kCollision->collision(player_, star);
	}
	kCollision->collision(player_, wall_);


	return this;
}

/*===========================================================================*/
// 星の生成
bool Endless::createStar()
{
	// 生成パターンの選択
	if (_chdir("State") == -1) { return false; }
	std::wstring pattern = pattern_file_[rand() % pattern_file_.size()];
	CsvLoader file(pattern);
	_chdir("../");


	Star* star;
	Vector2 position;
	float angle;
	float spin;
	float spin_rate;
	float size;

	int count = 1;
	while (true)
	{
		position.x     = file.getNumber_f(0, count);
		if (position.x == -1) { break; }
		position.y     = file.getNumber_f(1, count);
		angle          = file.getNumber_f(2, count);
		spin           = file.getNumber_f(3, count);
		spin_rate      = file.getNumber_f(4, count);
		size           = file.getNumber_f(5, count);


		position.y -= 720.0F;
		star = star_container_->addStar(
			position, angle, spin, spin_rate, size);
		if (star == nullptr)
		{
			return false;
		}
		star->setFall();
		++count;
	}
	return true;
}

/*===========================================================================*/
// オブジェクトの座標調整
void Endless::adjustObjectPosition(const float Over)
{
	if (Over > 0)
	{
		for (auto& background : background_)
		{
			background->setMove(Over);
		}
		back_object_->setMove(Over);

		for (auto& star : star_container_->active())
		{
			star->setMove(Over);
		}

		player_->setMove( Over );

		wall_->setMove( Over );
	}
}

/*===========================================================================*/
// スコアリング
void Endless::scoring()
{
	ObjectBase* const kPlayerOwner = player_->getOwner();
	const bool kPlayerJump = player_->isJump();

	// 前回の星と違う星に着地したときにコンボ
	if ((kPlayerJump == false && prev_player_jump_state_)&&
		(kPlayerOwner != prev_player_owner_))
	{
		combo_->addCombo();
	}

	if (kPlayerOwner) { prev_player_owner_ = kPlayerOwner; }
	prev_player_jump_state_ = kPlayerJump;
}
