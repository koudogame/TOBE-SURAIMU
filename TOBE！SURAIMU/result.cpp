
#include "result.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"

#include "title.h"

/*===========================================================================*/
Result::Result()
{
}

Result::~Result()
{
}


/*===========================================================================*/
// 初期化処理
bool Result::init()
{
	texture_ = TextureLoder::getInstance()->load(L"Texture/リザルト.png");
	if (texture_ == nullptr)	return false;

	return true;
}

/*===========================================================================*/
// 終了処理
void Result::destroy()
{
	TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
SceneBase* Result::update()
{
    if (Key::getInstance()->getTracker().pressed.Enter ||
        Pad::getInstance()->getTracker().b == GamePad::ButtonStateTracker::PRESSED)
    {
        return new Title;
    }

	return this;
}

/*===========================================================================*/
// 描画処理
void Result::draw()
{
	Sprite::getInstance()->draw(
		texture_, Vector2::Zero
	);
}
