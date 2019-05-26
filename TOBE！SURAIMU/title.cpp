
#include "title.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"
#include "endless.h"
#include "ranking.h"
#include "csvLoader.h"


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
	for( int i = 0; i < 5; i++ )
		object_[ i ] = std::make_unique<TitleObject>();

	scene_ = &Title::selectScene;

	//ロゴ
	RECT trim = { 0,0,getWindowWidth<int>(),getWindowHeight<int>() };
	object_status_[ ObjectNum::kRogo ].texture = TextureLoder::getInstance()->load( L"Texture/title.png" );
	object_status_[ ObjectNum::kRogo ].position = Vector2::Zero;
	object_status_[ ObjectNum::kRogo ].trim = trim;
	//カーソル
	trim.top = trim.bottom;
	trim.bottom += kMenuSize;
	trim.right = kMenuSize * 2;
	object_status_[ ObjectNum::kCusur ].texture = TextureLoder::getInstance()->load( L"Texture/title.png" );
	object_status_[ ObjectNum::kCusur ].position = Vector2( 385.0F , 350.0F );
	object_status_[ ObjectNum::kCusur ].trim = trim;

	//初期の星１
	trim.left = 150L;
	trim.top = 0L;
	trim.right = trim.left + 226L;
	trim.bottom = trim.top + 226L;
	object_status_[ ObjectNum::kStar1 ].texture = TextureLoder::getInstance()->load( L"Texture/star.png" );
	object_status_[ ObjectNum::kStar1 ].position = Vector2( 640.0F - 226.F / 2.0F , 600.0F - 226.F / 2.0F - getWindowHeight<float>() );
	object_status_[ ObjectNum::kStar1 ].trim = trim;

	//初期の星2
	trim.left = 150L + 226L;
	trim.top = 0L;
	trim.right = trim.left + 300L;
	trim.bottom = trim.top + 300L;
	object_status_[ ObjectNum::kStar2 ].texture = TextureLoder::getInstance()->load( L"Texture/star.png" );
	object_status_[ ObjectNum::kStar2 ].position = Vector2( 810.0F - 300.0F / 2.0F , 100.0F - 300.0F / 2.0F - getWindowHeight<float>() );
	object_status_[ ObjectNum::kStar2 ].trim = trim;

	//プレイヤー
	CsvLoader file( L"State/player_state.csv" );
	trim.left = 0;
	trim.top = 0;
	trim.right = trim.left + 46L;
	trim.bottom = trim.top + 46L;
	object_status_[ ObjectNum::kPlayer ].texture = TextureLoder::getInstance()->load( L"Texture/character.png" );
	object_status_[ ObjectNum::kPlayer ].position = Vector2( file.getNumber_f( 0 , 1 ) - 46.0F / 2.0F , file.getNumber_f( 1 , 1 ) - 46.0F / 2.0F - getWindowHeight<float>() );
	object_status_[ ObjectNum::kPlayer ].trim = trim;

	next_flag_ = false;

	for( int i = 0; i < 5; i++ )
		object_[ i ].get()->init( &object_status_[ i ] );

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
	for( int i = 0; i < 5; i++ )
		TextureLoder::getInstance()->release( object_status_[ i ].texture );
}

/*===========================================================================*/
// 更新処理
SceneBase* Title::update()
{
	return ( this->*scene_ )( );
}

/*===========================================================================*/
// 描画処理
void Title::draw()
{
	for( int i = 0; i < 5; i++ )
		object_[ i ].get()->draw( );
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

SceneBase* Title::playScene()
{
	volume_ -= 0.01F;
	scene_se_->setVolume( volume_ );
	title_bgm_->setVolume( volume_ );
	int itr = 0;
	for( itr; itr < 5; itr++ )
		object_[ itr ].get()->update();

	if( object_status_[ kRogo ].position.y > getWindowHeight<float>() )
	{
		title_bgm_->stop();
		scene_se_->stop();
		select_se_[ 0 ]->stop();
		select_se_[ 1 ]->stop();
		return new Endless;
	}
	return this;
}

SceneBase * Title::rankingScene()
{
	volume_ -= 0.01F;
	scene_se_->setVolume( volume_ );
	title_bgm_->setVolume( volume_ );
	for( int i = 0; i < 5; i++ )
		object_status_[ i ].alpha = volume_;

	if( volume_ < 0 )
		return new Ranking;
	return this;
}

SceneBase * Title::selectScene()
{
	pad_ = Pad::getInstance()->getTracker();
	pad_state_ = Pad::getInstance()->getState();
	key_ = Key::getInstance()->getTracker();

	input();

	if( key_.released.Space ||
		pad_.a == pad_.PRESSED )
	{
		scene_se_->play( AudioContainer::Mode::kDefault , true );
		select_se_[ 1 ]->play( AudioContainer::Mode::kDefault );

		switch( select_menu_ )
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
		object_status_[ ObjectNum::kCusur ].position.y = 350.0F + kCusurInterval * select_menu_;

	return this;
}
