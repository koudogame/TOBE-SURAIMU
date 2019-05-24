
#include "title.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"
#include "endless.h"


const int kMenuSize = 256;
const int kCusurInterval = 80;
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
	for( int i = 0; i < 2; i++ )
		object_[ i ] = std::make_unique<TitleObject>();

	//ロゴ
	RECT trim = { 0,0,getWindowWidth<int>(),getWindowHeight<int>() };
	object_[ ObjectNum::kRogo ].get()->init( Vector2::Zero , trim );
	//カーソル
	trim.top = trim.bottom;
	trim.bottom += kMenuSize;
	trim.right = kMenuSize * 2;
	object_[ ObjectNum::kCusur ].get()->init( Vector2( 385.0F , 350.0F ) , trim );

	next_flag_ = false;

	title_bgm_ = AudioLoader::getInstance()->getSound( L"Sound/title2-dova.wav" );
	select_se_[0] = AudioLoader::getInstance()->getSound( L"Sound/select1-dova.wav" );
	select_se_[1] = AudioLoader::getInstance()->getSound( L"Sound/select2-dova.wav" );
	scene_se_ = AudioLoader::getInstance()->getSound( L"Sound/scene1-dova.wav" );
	title_bgm_->allReset();
	title_bgm_->play( AudioContainer::Mode::kDefault );
	volume_ = 1.0F;
	return true;
}

/*===========================================================================*/
// 終了処理
void Title::destroy()
{
	for( int i = 0; i < 2; i++ )
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
		scene_se_->play( AudioContainer::Mode::kDefault , true );
		select_se_[ 1 ]->play( AudioContainer::Mode::kDefault );
		volume_ -= 0.01F;
		title_bgm_->setVolume( volume_ );
		int itr = 0;
		for( itr; itr < 2; itr++ )
			object_[ itr ].get()->update();
		for( itr = 0; itr < 2; itr++ )
			if( object_[ itr ].get()->isAlive() )
				break;
		if( itr == 2 )
		{
			title_bgm_->stop();
			scene_se_->stop();
			select_se_[ 0 ]->stop();
			select_se_[ 1 ]->stop();

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
		object_[ ObjectNum::kCusur ].get()->setPosition( Vector2( 385.0F , 350.0F + kCusurInterval * select_menu_ ) );

	return this;
}

/*===========================================================================*/
// 描画処理
void Title::draw()
{
	for( int i = 0; i < 2; i++ )
		object_[ i ].get()->draw();
}

void Title::input()
{
	//入力があったらカーソル反転
	if( ( key_.pressed.Down || pad_.leftStickDown == pad_.PRESSED ) ||
		( key_.pressed.Up || pad_.leftStickUp == pad_.PRESSED ) )
	{
		select_se_[0]->stop();
		select_se_[0]->play( AudioContainer::Mode::kDefault );
		select_menu_ = select_menu_ == Menu::kPlay ? Menu::kRanking : Menu::kPlay;
	}
}
