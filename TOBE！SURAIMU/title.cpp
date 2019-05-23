
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
// 初期化処理
bool Title::init()
{
	for( int i = 0; i < 3; i++ )
		object_[ i ] = std::make_unique<TitleObject>();

	//ロゴ
	RECT trim = { 0,0,getWindowWidth<int>(),543 };
	object_[ ObjectNum::kRogo ].get()->init( Vector2::Zero , trim );
	//メニュー
	trim.left = 0;
	trim.top = trim.bottom;
	trim.right = trim.left + kMenuSize;
	trim.bottom = trim.top + kMenuSize;
	object_[ ObjectNum::kMenu ].get()->init( Vector2( 542.0F , 374.0F ) , trim );
	//カーソル
	trim.left = trim.right;
	trim.right = trim.left + kMenuSize * 2;
	object_[ ObjectNum::kCusur ].get()->init( Vector2( 384.0F , 316.0F ) , trim );

	next_flag_ = false;

	return true;
}

/*===========================================================================*/
// 終了処理
void Title::destroy()
{
	for( int i = 0; i < 3; i++ )
		object_[ i ].get()->destroy();
}

/*===========================================================================*/
// 更新処理
SceneBase* Title::update()
{
	pad_ = Pad::getInstance()->getTracker();
	pad_state_ = Pad::getInstance()->getState();
	key_ = Key::getInstance()->getTracker();

	input();

	if( key_.released.Space ||
		pad_.a == pad_.PRESSED || next_flag_ )
	{
		int itr = 0;
		for( itr; itr < 3; itr++ )
			object_[ itr ].get()->update();
		for( itr = 0; itr < 3; itr++ )
			if( object_[ itr ].get()->isAlive() )
				break;
		if( itr == 3 )
		{
			switch( select_menu_ )
			{
				case Title::kPlay:
					return new Endless;
				case Title::kRanking:
					return nullptr;
			}
		}

		next_flag_ = true;
	}
	else
		object_[ ObjectNum::kCusur ].get()->setPosition( Vector2( 384.0F , 316.0F + kCusurInterval * select_menu_ ) );

	return this;
}

/*===========================================================================*/
// 描画処理
void Title::draw()
{
	for( int i = 0; i < 3; i++ )
		object_[ i ].get()->draw();
}

void Title::input()
{
	//入力があったらカーソル反転
	if(( key_.pressed.Down || pad_state_.thumbSticks.leftY < -0.3F )||
		(key_.pressed.Up || pad_state_.thumbSticks.leftY > 0.3F) )
	{
		select_menu_ = select_menu_ == Menu::kPlay ? Menu::kRanking : Menu::kPlay;
	}
}
