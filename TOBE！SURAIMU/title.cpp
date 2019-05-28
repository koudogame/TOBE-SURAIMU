
#include "title.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"
#include "endless.h"
#include "ranking.h"
#include "csvLoader.h"
#include "easing.h"


const int kMenuSize = 256;
const int kCusurInterval = 80;
const int kLayer = 3;
const int kFall = 6;
const int kWallWidth = 32;
const float kWallHeight = 88.0F;


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
	for( int i = 0; i < ObjectNum::kObjectNum; i++ )
		object_[ i ] = std::make_unique<TitleObject>();

	scene_ = &Title::selectScene;

	//ロゴ
	RECT trim = { 0,0,getWindowWidth<int>(),getWindowHeight<int>() };
	object_status_[ ObjectNum::kRogo ].texture = TextureLoder::getInstance()->load( L"Texture/title.png" );
	object_status_[ ObjectNum::kRogo ].position = Vector2::Zero;
	object_status_[ ObjectNum::kRogo ].trim = trim;
	//カーソル
	//マジックナンバー
	trim.top = trim.bottom;
	trim.bottom += kMenuSize;
	trim.right = kMenuSize * 2;
	object_status_[ ObjectNum::kCusur ].texture = TextureLoder::getInstance()->load( L"Texture/title.png" );
	object_status_[ ObjectNum::kCusur ].position = Vector2( 385.0F , 350.0F );
	object_status_[ ObjectNum::kCusur ].trim = trim;

	//初期の星１
	//マジックナンバー
	trim.left = 150L;
	trim.top = 0L;
	trim.right = trim.left + 226L;
	trim.bottom = trim.top + 226L;
	object_status_[ ObjectNum::kStar1 ].texture = TextureLoder::getInstance()->load( L"Texture/star.png" );
	object_status_[ ObjectNum::kStar1 ].position = Vector2( 640.0F - 226.F / 2.0F , 600.0F - 226.F / 2.0F - getWindowHeight<float>() );
	object_status_[ ObjectNum::kStar1 ].trim = trim;

	//初期の星2
	//マジックナンバー
	trim.left = 150L + 226L;
	trim.top = 0L;
	trim.right = trim.left + 300L;
	trim.bottom = trim.top + 300L;
	object_status_[ ObjectNum::kStar2 ].texture = TextureLoder::getInstance()->load( L"Texture/star.png" );
	object_status_[ ObjectNum::kStar2 ].position = Vector2( 810.0F - 300.0F / 2.0F , 100.0F - 300.0F / 2.0F - getWindowHeight<float>() );
	object_status_[ ObjectNum::kStar2 ].trim = trim;

	//プレイヤー
	//マジックナンバー
	CsvLoader file( L"State/player_state.csv" );
	trim.left = 0;
	trim.top = 0;
	trim.right = trim.left + 46L;
	trim.bottom = trim.top + 46L;
	object_status_[ ObjectNum::kPlayer ].texture = TextureLoder::getInstance()->load( L"Texture/character.png" );
	object_status_[ ObjectNum::kPlayer ].position = Vector2( file.getNumber_f( 0 , 1 ) - 46.0F / 2.0F , file.getNumber_f( 1 , 1 ) - 46.0F / 2.0F - getWindowHeight<float>() );
	object_status_[ ObjectNum::kPlayer ].trim = trim;

	//壁( 右 )
	trim.left = 0L;
	trim.top = 0L;
	trim.right = trim.left + kWallWidth;
	trim.bottom = trim.top + static_cast<int>(kWallHeight);
	object_status_[ ObjectNum::kWallRight ].texture = TextureLoder::getInstance()->load( L"Texture/wall.png" );
	object_status_[ ObjectNum::kWallRight ].position = Vector2( getWindowWidth<float>(), 0.0F );
	object_status_[ ObjectNum::kWallRight ].trim = trim;

	//壁( 左 )
	trim.left = 0L;
	trim.top = 0L;
	trim.right = trim.left + kWallWidth;
	trim.bottom = trim.top + static_cast<int>(kWallHeight);
	object_status_[ ObjectNum::kWallLeft ].texture = TextureLoder::getInstance()->load( L"Texture/wall.png" );
	object_status_[ ObjectNum::kWallLeft ].position = Vector2( -kWallWidth , 0.0F );
	object_status_[ ObjectNum::kWallLeft ].trim = trim;


	next_flag_ = false;

	for( int i = 0; i < ObjectNum::kObjectNum; i++ )
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
	for( int i = 0; i < ObjectNum::kObjectNum; i++ )
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
	for( int i = 0; i <= ObjectNum::kPlayer; i++ )
		object_[ i ].get()->draw();

	for( float y = object_status_[ ObjectNum::kWallRight ].position.y; y < getWindowHeight<float>(); y += kWallHeight )
	{
		Vector2 positright = object_status_[ ObjectNum::kWallRight ].position;
		Vector2 positleft = object_status_[ ObjectNum::kWallLeft ].position;

		positright.y = y;
		positleft.y = y;

		Sprite::getInstance()->draw( object_status_[ ObjectNum::kWallRight ].texture , positright , &object_status_[ ObjectNum::kWallRight ].trim , object_status_[ ObjectNum::kWallRight ].alpha ,
									 1.0F , Vector2( 1.0F , 1.0F ) , 0.0F , Vector2::Zero , SpriteEffects::SpriteEffects_FlipHorizontally );
		Sprite::getInstance()->draw( object_status_[ ObjectNum::kWallLeft ].texture , positleft , &object_status_[ ObjectNum::kWallLeft ].trim , object_status_[ ObjectNum::kWallLeft ].alpha ,
									 1.0F , Vector2( 1.0F , 1.0F ) , 0.0F , Vector2::Zero , SpriteEffects::SpriteEffects_None );
	}
}

void Title::input()
{
	//入力があったらカーソル反転
	if( key_.pressed.Down || pad_.leftStickDown == pad_.PRESSED)
	{
		if( select_menu_ == Menu::kPlay )
		{
		select_se_[ 0 ]->stop();
		select_se_[ 0 ]->resetPitch();
		select_se_[0]->play( AudioContainer::Mode::kDefault );
			select_menu_ = Menu::kRanking;
		}
		else
		{
			select_se_[ 0 ]->stop();
			select_se_[ 0 ]->setPitch( -0.5F );
			select_se_[ 0 ]->play( AudioContainer::Mode::kDefault );
		}
	}
	else if( key_.pressed.Up || pad_.leftStickUp == pad_.PRESSED )
	{
		if( select_menu_ == Menu::kRanking )
		{
			select_se_[ 0 ]->stop();
			select_se_[ 0 ]->resetPitch();
			select_se_[ 0 ]->play( AudioContainer::Mode::kDefault );
			select_menu_ = Menu::kPlay;
		}
		else
		{
			select_se_[ 0 ]->stop();
			select_se_[ 0 ]->setPitch(-0.5F);
			select_se_[ 0 ]->play( AudioContainer::Mode::kDefault );
		}

	}
}

SceneBase* Title::playScene()
{
	volume_ -= 0.01F;
	scene_se_->setVolume( volume_ );
	title_bgm_->setVolume( volume_ );
	for( int i = 0; i <= ObjectNum::kPlayer; i++ )
		object_status_[ i ].position.y += kFall;

	float now_time_ = object_status_[ ObjectNum::kRogo ].position.y / getWindowHeight<float>();
	if( now_time_ >= 1.0F )
		now_time_ = 1.0F;
	object_status_[ kWallRight ].position.x = getWindowWidth<float>() - Easing::getInstance()->expo( 320.0F + kWallWidth / 2.0F , now_time_ , Easing::Mode::In );
	object_status_[ kWallLeft ].position.x = -kWallWidth + Easing::getInstance()->expo( 320.0F + kWallWidth / 2.0F, now_time_ , Easing::Mode::In );

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
	for( int i = 0; i < ObjectNum::kObjectNum; i++ )
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
		scene_se_->stop();
		select_se_[ 1 ]->stop();
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
