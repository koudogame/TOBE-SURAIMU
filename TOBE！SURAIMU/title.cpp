
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
	{
		TextureLoder::getInstance()->release(object_status_[i].texture);
		TextureLoder::getInstance()->release(object_status_[i].overlay_texture);
	}
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
	for (int i = 0; i < ObjectNum::kObjectNum; i++)
		object_status_[i].draw();
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
	SOUND->setVolume(SoundId::kScene, volume_);
	SOUND->play(SoundId::kScene, false);
	for (int i = 0; i < ObjectNum::kObjectNum; i++)
		object_status_[i].alpha = volume_;


	if (volume_ < 0)
	{

		SOUND->stop(SoundId::kScene);
		SOUND->stop(SoundId::kTitle);
		SOUND->setVolume(SoundId::kTitle, 1.0F);
		SOUND->stop(SoundId::kScene);
		return new Endless;
	}
	return this;
}

SceneBase * Title::rankingScene()
{
	volume_ -= 0.01F;
	SOUND->setVolume(SoundId::kScene, volume_);
	SOUND->play(SoundId::kScene, false);
	for (int i = 0; i < ObjectNum::kObjectNum; i++)
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

	if (volume_ < 0)
	{
		return new Demo;
	}
	return this;
}
