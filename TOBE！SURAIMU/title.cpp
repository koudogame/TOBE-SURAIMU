
#include "title.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"
#include "endless.h"
#include "ranking.h"
#include "csvLoader.h"
#include "easing.h"
#include "Sound.h"

#include "demo.h"
#include "stage_select.h"

using namespace std::chrono;

const int kMenuSize = 256;
const int kCusurInterval = 80;
const int kLayer = 3;
const int kFall = 6;
const int kWallWidth = 32;
const float kWallHeight = 88.0F;
const long kDemoTimeSecond = 30;
const Vector2 kObjectposition[] =
{
	Vector2::Zero,
	Vector2(385.0F , 350.0F),
	Vector2(600.0F - 46.0F / 2.0F , 565.0F - 46.0F / 2.0F - getWindowHeight<float>()),
	Vector2(640.0F - 226.F / 2.0F , 600.0F - 226.F / 2.0F - getWindowHeight<float>()),
	Vector2(816.0F - 300.0F / 2.0F , 297.0F - 300.0F / 2.0F - getWindowHeight<float>()),
	Vector2(465.0F - 300.0F / 2.0F , 142.0F - 300.0F / 2.0F - getWindowHeight<float>()),
	Vector2(465.0F - 300.0F / 2.0F , 142.0F - 300.0F / 2.0F - getWindowHeight<float>()),
	Vector2(getWindowWidth<float>(), 0.0F),
	Vector2(-kWallWidth , 0.0F)
};
const RECT kObjectTrim[] =
{
	{ 0,0,getWindowWidth<int>(),getWindowHeight<int>() },
	{0,getWindowHeight<int>(),kMenuSize * 2,getWindowWidth<int>() + kMenuSize},
	{0,0,46,46},
	{150,0,150 + 226,226},
	{150 + 226,0,150 + 226 + 300,300},
	{150 + 226,0,150 + 226 + 300,300},
	{150 + 226,0,150 + 226 + 300,300},
	{0,0,static_cast<long>(kWallWidth),static_cast<long>(kWallHeight)},
	{0,0,static_cast<long>(kWallWidth),static_cast<long>(kWallHeight)},
	{ 0,0,getWindowWidth<int>(),getWindowHeight<int>() },
};

const Vector3 kStarInformation[3] = {	//星の切り取り情報( x,y,size )
	Vector3(0.0F,0.0F,150.0F),
	Vector3(150.0F,0.0F,226.0F),
	Vector3(150.0F + 226.0F,0.0F,300.F)
};
const int kStarMin = 60;			//最小サイズ
const int kStarDifference = 20;		//星のサイズの差分
const float kStarColorThreshold[2] =		//色変更の閾値
{
	0.5F,
	0.8F
};
const int kBackParticleSize = 46;
const int kCreateStarNum = 8;
/*===========================================================================*/
Title::Title()
{
}

Title::~Title()
{
}


/*===========================================================================*/
// 初期化処理
bool Title::init()
{
	first_stage_.load(L"State/start_pattern.csv");

	scene_ = &Title::selectScene;

	//ロゴ
	object_status_[ObjectNum::kRogo].texture = TextureLoder::getInstance()->load(L"Texture/title.png");
	object_status_[ObjectNum::kRogo].position = kObjectposition[ObjectNum::kRogo];;
	object_status_[ObjectNum::kRogo].trim = kObjectTrim[ObjectNum::kRogo];
	object_status_[ObjectNum::kRogo].depth = 30.0F;
	//カーソル
	object_status_[ObjectNum::kCusur].texture = TextureLoder::getInstance()->load(L"Texture/title.png");
	object_status_[ObjectNum::kCusur].position = kObjectposition[ObjectNum::kCusur];
	object_status_[ObjectNum::kCusur].trim = kObjectTrim[ObjectNum::kCusur];
	object_status_[ObjectNum::kCusur].depth = 30.0F;

	//初期の星１
	for (int i = 0; i < kCreateStarNum; ++i)
	{
		star_obj_.push_back(TitleStatus());
		star_obj_.back().texture = TextureLoder::getInstance()->load(L"Texture/star.png");
		star_obj_.back().position.x = first_stage_.getNumber(0, i + 1);
		star_obj_.back().position.y = first_stage_.getNumber(1, i + 1) - getWindowHeight<float>() * 2.0F;
		Vector3 inf = kStarInformation[(static_cast<int>(first_stage_.getNumber(5, 1)) - kStarMin) / kStarDifference];
		float rate = first_stage_.getNumber_f(4, i + 1);
		int id = rate < kStarColorThreshold[0] ? 0 : (rate < kStarColorThreshold[1] ? 1 : 2);
		star_obj_.back().trim.left = inf.x;
		star_obj_.back().trim.top = inf.y + id * inf.z;
		star_obj_.back().trim.right = star_obj_.back().trim.left + inf.z;
		star_obj_.back().trim.bottom = star_obj_.back().trim.top + inf.z;
	}

	//プレイヤー
	object_status_[ObjectNum::kPlayer].texture = TextureLoder::getInstance()->load(L"Texture/character.png");
	object_status_[ObjectNum::kPlayer].position = kObjectposition[ObjectNum::kPlayer];
	object_status_[ObjectNum::kPlayer].trim = kObjectTrim[ObjectNum::kPlayer];
	object_status_[ObjectNum::kPlayer].depth = 30.0F;

	//壁( 右 )
	object_status_[ObjectNum::kWallRight].texture = TextureLoder::getInstance()->load(L"Texture/wall.png");
	object_status_[ObjectNum::kWallRight].position = kObjectposition[ObjectNum::kWallRight];
	object_status_[ObjectNum::kWallRight].trim = kObjectTrim[ObjectNum::kWallRight];

	//壁( 左 )
	object_status_[ObjectNum::kWallLeft].texture = TextureLoder::getInstance()->load(L"Texture/wall.png");
	object_status_[ObjectNum::kWallLeft].position = kObjectposition[ObjectNum::kWallLeft];
	object_status_[ObjectNum::kWallLeft].trim = kObjectTrim[ObjectNum::kWallLeft];
	//黒背景
	object_status_[ObjectNum::kBlack].texture = TextureLoder::getInstance()->load(L"Texture/black.png");
	object_status_[ObjectNum::kBlack].position = Vector2::Zero;
	object_status_[ObjectNum::kBlack].trim = kObjectTrim[ObjectNum::kBlack];
	object_status_[ObjectNum::kBlack].alpha = 0.0F;
	object_status_[ObjectNum::kBlack].depth = 100.0F;

	overlay_texture_ = TextureLoder::getInstance()->load(L"Texture/star1.png");

	volume_ = 1.0F;

	//サウンドの生成
	SOUND->play(SoundId::kTitle, true);
	timer_.start();
	return true;
}

/*===========================================================================*/
// 終了処理
void Title::destroy()
{
	for (int i = 0; i < ObjectNum::kObjectNum; i++)
		TextureLoder::getInstance()->release(object_status_[i].texture);

	TextureLoder::getInstance()->release(overlay_texture_);
}

/*===========================================================================*/
// 更新処理
SceneBase* Title::update()
{
	return (this->*scene_)();
}

/*===========================================================================*/
// 描画処理
void Title::draw()
{
	for (auto& itr : star_obj_)
	{
		Vector2 anker = Vector2((itr.trim.right - itr.trim.left) / 2.0F, (itr.trim.bottom - itr.trim.top) / 2.0F);
		Sprite::getInstance()->reserveDraw(overlay_texture_, itr.position, itr.trim, 1.0F, 13.0F, { 1.0F, 1.0F }, 0.0F, anker, Sprite::getInstance()->chengeMode());
		Sprite::getInstance()->reserveDraw(itr.texture, itr.position, itr.trim, 1.0F, 14.0F, { 1.0F, 1.0F }, 0.0F, anker);
	}

	//パーティクル用切り取り位置
	RECT trim;
	trim.left = kBackParticleSize * 2;
	trim.top = 0;
	trim.right = trim.left + kBackParticleSize;
	trim.bottom = trim.top + kBackParticleSize;
	Sprite::getInstance()->reserveDraw(object_status_[ObjectNum::kPlayer].texture, object_status_[ObjectNum::kPlayer].position, trim, 1.0F, object_status_[ObjectNum::kPlayer].depth - 0.1F);

	for (int i = 0; i <= ObjectNum::kPlayer; i++)
		object_status_[i].draw();

	for (float y = object_status_[ObjectNum::kWallRight].position.y; y < getWindowHeight<float>(); y += kWallHeight)
	{
		Vector2 positright = object_status_[ObjectNum::kWallRight].position;
		Vector2 positleft = object_status_[ObjectNum::kWallLeft].position;

		positright.y = y;
		positleft.y = y;

		Sprite::getInstance()->reserveDraw(object_status_[ObjectNum::kWallRight].texture, positright, object_status_[ObjectNum::kWallRight].trim, object_status_[ObjectNum::kWallRight].alpha,
			30.0F, Vector2(1.0F, 1.0F), 0.0F, Vector2::Zero, Common::getInstance()->getStates()->NonPremultiplied(), SpriteEffects::SpriteEffects_FlipHorizontally);
		Sprite::getInstance()->reserveDraw(object_status_[ObjectNum::kWallLeft].texture, positleft, object_status_[ObjectNum::kWallLeft].trim, object_status_[ObjectNum::kWallLeft].alpha,
			30.0F, Vector2(1.0F, 1.0F), 0.0F, Vector2::Zero, Common::getInstance()->getStates()->NonPremultiplied(), SpriteEffects::SpriteEffects_None);
	}

	object_status_[ObjectNum::kBlack].draw();


}

void Title::input()
{
	//入力があったらカーソル反転
	if (key_.pressed.Down || pad_.dpadDown == pad_.PRESSED || pad_.leftStickRight == pad_.PRESSED)
	{
		if (select_menu_ == Menu::kPlay)
		{
			select_menu_ = Menu::kRanking;
			SOUND->setPitch(SoundId::kSelect, 0.0F);
			SOUND->stop(SoundId::kSelect);
			SOUND->play(SoundId::kSelect, false);
		}
		else
		{
			SOUND->setPitch(SoundId::kSelect, -0.5F);
			SOUND->stop(SoundId::kSelect);
			SOUND->play(SoundId::kSelect, false);
		}
		timer_.start();
	}
	else if (key_.pressed.Up || pad_.dpadUp == pad_.PRESSED || pad_.leftStickLeft == pad_.PRESSED)
	{
		if (select_menu_ == Menu::kRanking)
		{
			select_menu_ = Menu::kPlay;
			SOUND->setPitch(SoundId::kSelect, 0.0F);
			SOUND->stop(SoundId::kSelect);
			SOUND->play(SoundId::kSelect, false);
		}
		else
		{
			SOUND->setPitch(SoundId::kSelect, -0.5F);
			SOUND->stop(SoundId::kSelect);
			SOUND->play(SoundId::kSelect, false);
		}
		timer_.start();
	}
}

SceneBase* Title::playScene()
{
	volume_ -= 0.01F;
	if (volume_ < 0.0F)volume_ = 0.0F;
	SOUND->setVolume(SoundId::kScene, volume_);
	SOUND->setVolume(SoundId::kTitle, volume_);
	SOUND->play(SoundId::kScene, false);

	for (auto& itr : star_obj_)
		itr.position.y += kFall;

	for (int i = 0; i <= ObjectNum::kPlayer; ++i)
		object_status_[i].position.y += kFall;

	float now_time_ = object_status_[ObjectNum::kRogo].position.y / getWindowHeight<float>();
	if (now_time_ >= 1.0F)
		now_time_ = 1.0F;
	object_status_[kWallRight].position.x = getWindowWidth<float>() - Easing::getInstance()->expo(getWindowWidth<float>() / 4.0F + kWallWidth / 2.0F, now_time_, Easing::Mode::In);
	object_status_[kWallLeft].position.x = -kWallWidth + Easing::getInstance()->expo(320.0F + kWallWidth / 2.0F, now_time_, Easing::Mode::In);

	if (object_status_[kRogo].position.y > getWindowHeight<float>())
	{
		SOUND->stop(SoundId::kScene);
		SOUND->stop(SoundId::kTitle);
		SOUND->setVolume(SoundId::kTitle, 1.0F);
		return new StageSelect();
	}
	return this;
}

SceneBase * Title::rankingScene()
{
	volume_ -= 0.01F;
	SOUND->setVolume(SoundId::kScene, volume_);
	SOUND->play(SoundId::kScene, false);
	for (int i = 0; i < ObjectNum::kBlack; i++)
		object_status_[i].alpha = volume_;


	if (volume_ < 0)
	{
		SOUND->stop(SoundId::kScene);
		return new Ranking;
	}
	return this;
}

SceneBase * Title::selectScene()
{
	pad_ = Pad::getInstance()->getTracker();
	pad_state_ = Pad::getInstance()->getState();
	key_ = Key::getInstance()->getTracker();

	input();

	if (key_.released.Space ||
		pad_.a == pad_.RELEASED)
	{
		SOUND->stop(SoundId::kDicision);
		SOUND->play(SoundId::kDicision, false);
		switch (select_menu_)
		{
		case Title::kPlay:
			scene_ = &Title::playScene;
			break;
		case Title::kRanking:
			scene_ = &Title::rankingScene;
			break;
		}
	}
	else
		object_status_[ObjectNum::kCusur].position.y = kObjectposition[ObjectNum::kCusur].y + kCusurInterval * select_menu_;

	if (timer_.getCount() >= kDemoTimeSecond)
		scene_ = &Title::DemoScene;

	return this;
}

SceneBase * Title::DemoScene()
{
	volume_ -= 0.01F;
	for (int i = 0; i < ObjectNum::kObjectNum; i++)
		object_status_[i].alpha = volume_;
	object_status_[ObjectNum::kBlack].alpha = 1.0F - volume_;


	if (volume_ < 0)
	{
		return new Demo;
	}
	return this;
}
