
#include "title.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"
#include "endless.h"


const int kMenuSize = 256;
const int kCusurInterval = 110;
const int kLayer = 3;


/*===========================================================================*/
Title::Title()
{
}

Title::~Title()
{
}


/*===========================================================================*/
// ‰Šú‰»ˆ—
bool Title::init()
{
	texture_ = TextureLoder::getInstance()->load(L"Texture/title.png");

	//”wŒi‚Ì¶¬
	RECT trim = { 0,0,1024,1024 };

	return true;
}

/*===========================================================================*/
// I—¹ˆ—
void Title::destroy()
{
	TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// XVˆ—
SceneBase* Title::update()
{
	pad_ = Pad::getInstance()->getTracker();
	pad_state_ = Pad::getInstance()->getState();
	key_ = Key::getInstance()->getTracker();

	input();

	if ( key_.released.Space ||
		 pad_.a == pad_.PRESSED)
	{
		switch( select_menu_ )
		{
			case Title::kPlay:
				return new Endless;
			case Title::kRanking:
				return nullptr;
		}
	}

	return this;
}

/*===========================================================================*/
// •`‰æˆ—
void Title::draw()
{
	//ƒƒS‚Ì•`‰æ
	RECT trim = { 0,0,getWindowWidth<int>(),543 };
	Sprite::getInstance()->draw(
		texture_,
		Vector2::Zero,
		&trim
	);

	//ƒƒjƒ…[‚Ì•`‰æ
	trim.left = 0;
	trim.top = trim.bottom;
	trim.right = trim.left + kMenuSize;
	trim.bottom = trim.top + kMenuSize;
	Sprite::getInstance()->draw(
		texture_ ,
		Vector2( 542.0F , 374.0F ) ,
		&trim
	);

	//ƒJ[ƒ\ƒ‹‚Ì•`‰æ
	trim.left = trim.right;
	trim.right = trim.left + kMenuSize * 2;
	Sprite::getInstance()->draw(
		texture_ ,
		Vector2( 384.0F , 316.0F + kCusurInterval * select_menu_ ) ,
		&trim
	);
}

void Title::input()
{
	//“ü—Í‚ª‚ ‚Á‚½‚çƒJ[ƒ\ƒ‹”½“]
	if(( key_.pressed.Down || pad_state_.thumbSticks.leftY < -0.3F )||
		(key_.pressed.Up || pad_state_.thumbSticks.leftY > 0.3F) )
	{
		select_menu_ = select_menu_ == Menu::kPlay ? Menu::kRanking : Menu::kPlay;
	}
}
