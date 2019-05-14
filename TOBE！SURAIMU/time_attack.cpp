
#include "time_attack.h"

#include <direct.h>
#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"
#include "result.h"
#include "task_manager.h"
#include "collision.h"
#include "star_container.h"
#include "star.h"
#include "player.h"
#include "wall.h"

using namespace std::chrono;

using PadState = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;


/*===========================================================================*/
// 難易度に関係
constexpr long long kLimitTimeSec = 60LL; // 制限時間( 秒 )
constexpr float kScoringAngle = 3.6F;     // スコアリング角度( 度数法 )

// 処理に関係
constexpr RECT kTrimmingBackground{       // 背景切り取り範囲
	0L, 720L, 1280L, 1440L};
constexpr RECT kTrimmingEffect{           // 背景エフェクト切り取り範囲
	0L,   0L, 1280L,  720L};
constexpr long kNumberWidth = 64L;        // 数字横幅
constexpr long kNumberHeight = 128L;      // 数字縦幅



/*===========================================================================*/
TimeAttack::TimeAttack() :
	SceneBase(SceneID::kPlay)
{
}

TimeAttack::~TimeAttack()
{
	// 開放忘れ対処
	if (do_create_ == false)
	{
		destroy();
	}
}

/*===========================================================================*/
// 初期化処理
bool TimeAttack::init()
{
	// 必要オブジェクトの生成
	if (do_create_ && create() == false) { return false; }

	// スターを生成
	if (createStar() == false) { return false; }


	// プレイヤー初期化
	Vector2 position;
	float jump;
	float add_vol;
	float decay;
	float gravity;
	float speed;
	float up_boost;
	float rl_boost;
	FILE* player_state = nullptr;
	errno_t error = fopen_s(&player_state, "State/player_state.txt", "r");
	if (error != 0) { return false; }
	fscanf_s(player_state,
		"%f %f %f %f %f %f %f %f %f",
		&position.x, &position.y,
		&jump,
		&add_vol,
		&decay,
		&gravity,
		&speed,
		&up_boost, &rl_boost);
	fclose(player_state);
	if (player_->init(position, jump, add_vol, decay, gravity, speed, up_boost, rl_boost) == false)
	{
		return false;
	}


	// 壁の初期化
	if (wall_->init() == false) { return false; }


	// その他メンバ
	update_ = &TimeAttack::start;
	remaining_time_sec_ = kLimitTimeSec;
	score_ = 0;
	player_rotate_sum_ = 0.0F;

	return true;
}
// 生成処理
bool TimeAttack::create()
{
	do_create_ = false;

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

	// テクスチャ
	texture_ = TextureLoder::getInstance()->load(L"Texture/プレイ画面.png");
	if (texture_ == nullptr) { return false; }
	texture_numbers_ = TextureLoder::getInstance()->load(L"Texture/数字.png");
	if (texture_ == nullptr) { return false; }


	// タスクマネージャー
	task_manager_   = new (std::nothrow) TaskManager();
	if (task_manager_ == nullptr)   { return false; }

	// スターコンテナ
	star_container_ = new (std::nothrow) StarContainer(task_manager_);
	if (star_container_ == nullptr) { return false; }

	// プレイヤー
	player_         = new (std::nothrow) Player(task_manager_);
	if (player_ == nullptr)         { return false; }

	// 壁
	wall_           = new (std::nothrow) Wall(task_manager_);
	if (wall_ == nullptr)           { return false; }

	return true;
}

/*===========================================================================*/
// 終了処理
void TimeAttack::destroy()
{
	do_create_ = true;

	// 壁
	wall_->destroy();
	safe_delete(wall_);

	// プレイヤー
	player_->destroy();
	safe_delete(player_);

	// 星々
	star_container_->destroy();
	safe_delete(star_container_);

	// タスクマネージャー
	safe_delete(task_manager_);

	// テクスチャ
	TextureLoder::getInstance()->release(texture_numbers_);
	TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
SceneBase* TimeAttack::update()
{
	return (this->*update_)();
}

/*===========================================================================*/
// 描画処理
void TimeAttack::draw()
{
	// 背景の描画
	Sprite::getInstance()->draw(
		texture_, Vector2::Zero, &kTrimmingBackground);
	Sprite::getInstance()->draw(
		texture_, Vector2::Zero, &kTrimmingEffect);

	// オブジェクトの描画
	task_manager_->allExecuteDraw();

	// 残り時間描画
	remaining_time_sec_.draw(
		texture_numbers_,
		Vector2(1280.0F, 0.0F),
		kNumberWidth, kNumberHeight
	);

	// スコア描画
	score_.draw(
		texture_numbers_,
		Vector2(320.0F, 0.0F),
		kNumberWidth, kNumberHeight
	);
}

/*===========================================================================*/
// スタート
SceneBase* TimeAttack::start()
{
	if (player_->isJump())
	{
		update_ = &TimeAttack::play;
		star_container_->setFall();
		prev_time_ = high_resolution_clock::now();
	}

	task_manager_->allExecuteUpdate();

	return this;
}

/*===========================================================================*/
// プレイ
SceneBase* TimeAttack::play()
{
	// タイム管理
	// 秒で管理しているため、時間の変化があるまで更新をしない
	auto now = high_resolution_clock::now();
	auto delta_sec = duration_cast<seconds>(now - prev_time_).count();
	if (delta_sec > 0)
	{
		remaining_time_sec_ -= delta_sec;
		prev_time_ = now;
		if (remaining_time_sec_ <= 0)
		{
			// ゲーム終了
			return new Result;
		}
	}


	if (Key::getInstance()->getTracker().pressed.P ||
		Pad::getInstance()->getTracker().menu == PadTracker::PRESSED)
	{
		// ポーズ画面へ
		update_ = &TimeAttack::pause;
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
		if (itr == end)	{
			if (createStar() == false)
			{
			// 生成失敗
				return nullptr; 
			}
			else
			{
			// 生成成功->落下を付与
				star_container_->setFall(); 
			}
		}


		// オブジェクトの更新処理
		task_manager_->allExecuteUpdate();
		scoring();


		// 衝突判定
		Collision* const kCollision = Collision::getInstance();
		for (auto& star : star_container_->active()) // 対スター
		{
			kCollision->collision(player_, star);
		}
		kCollision->collision(player_, wall_);       // 対壁
	}


	return this;
}

/*===========================================================================*/
// ポーズ
SceneBase* TimeAttack::pause()
{
	if (Key::getInstance()->getTracker().pressed.Enter ||
		Pad::getInstance()->getTracker().b == PadTracker::PRESSED)
	{
		// ゲーム終了
		return new Result;
	}

	if (Key::getInstance()->getTracker().pressed.P ||
		Pad::getInstance()->getTracker().menu == PadTracker::PRESSED)
	{
		// プレイ続行
		update_ = &TimeAttack::play;
	}


	return this;
}

/*===========================================================================*/
// 星の生成
bool TimeAttack::createStar()
{
	// 生成パターンファイルを選択
	_chdir("State");
	unsigned file_num = rand() % star_pattern_list_.size();
	errno_t error = fopen_s(
		&star_pattern_, star_pattern_list_[file_num].c_str(), "r");
	_chdir("../");
	if (error != 0) { return false; }


	// 星の生成
	Vector2 position;
	float angle;
	float fall;
	float spin;
	float spin_rate;
	float size;
	while (true)
	{
		// ファイルから生成情報を取得
		if (fscanf_s(star_pattern_,
			"%f %f %f %f %f %f %f",
			&position.x, &position.y,
			&angle,
			&fall,
			&spin,
			&spin_rate,
			&size) == EOF)
		{
			// ファイル終端に到達
			return true;
		}

		position.y -= getWindowHeight<float>(); // 画面外へ追いやる
		if (star_container_->addStar(
			position, angle, fall, spin, spin_rate, size) == nullptr)
		{
			// 生成に失敗
			return false;
		}
	}
	fclose(star_pattern_);

	return true;
}

/*===========================================================================*/
// スコアリング
void TimeAttack::scoring()
{
	const float kPlayerRotate = player_->getRotate() * 180.0F / XM_PI;

	// 回転が止まったら加算終了
	if (kPlayerRotate == 0.0F)
	{
		player_rotate_sum_ = 0.0F;
	}
	// 回転中はスコア加算
	else
	{
		player_rotate_sum_ += kPlayerRotate;
		const int kScore = static_cast<int>(player_rotate_sum_ / kScoringAngle);
		if (kScore > 0)
		{
			score_ += kScore;
			player_rotate_sum_ = 0.0F;
		}
	}
}
