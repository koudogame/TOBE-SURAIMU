
#include "title.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"
#include "time_attack.h"

using PadState = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;

/*===========================================================================*/
Title::Title() :
	SceneBase(SceneID::kTitle)
{
}

Title::~Title()
{
}


/*===========================================================================*/
// 初期化処理
bool Title::init()
{
	texture_ = TextureLoder::getInstance()->load(L"Texture/タイトル.png");

	return true;
}

/*===========================================================================*/
// 終了処理
void Title::destroy()
{
	TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
SceneBase* Title::update()
{
	if (Key::getInstance()->getTracker().pressed.Enter ||
		Pad::getInstance()->getTracker().b == PadTracker::PRESSED)
	{
		return new TimeAttack();
	}

	return this;
}

/*===========================================================================*/
// 描画処理
void Title::draw()
{
	Sprite::getInstance()->draw(
		texture_,
		Vector2::Zero
	);
}
