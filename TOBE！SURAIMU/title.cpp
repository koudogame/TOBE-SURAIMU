
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


const int kMenuSize = 256;
const int kCusurInterval = 80;
const int kLayer = 3;
const int kFall = 6;
const int kWallWidth = 32;
const float kWallHeight = 88.0F;
const Vector2 kObjectposition[] =
{
	Vector2::Zero,
	Vector2( 385.0F , 350.0F ),
	Vector2( 600.0F - 46.0F / 2.0F , 565.0F - 46.0F / 2.0F - getWindowHeight<float>() ),
	Vector2( 640.0F - 226.F / 2.0F , 600.0F - 226.F / 2.0F - getWindowHeight<float>() ),
	Vector2( 816.0F - 300.0F / 2.0F , 297.0F - 300.0F / 2.0F - getWindowHeight<float>() ),
	Vector2( 465.0F - 300.0F / 2.0F , 142.0F - 300.0F / 2.0F - getWindowHeight<float>() ),
	Vector2( getWindowWidth<float>(), 0.0F ),
	Vector2( -kWallWidth , 0.0F )
};
const RECT kObjectTrim[] =
{
	{ 0,0,getWindowWidth<int>(),getWindowHeight<int>() },
	{0,getWindowHeight<int>(),kMenuSize * 2,getWindowWidth<int>() + kMenuSize},
	{0,0,46,46},
	{150,0,150 + 226,226},
	{150 + 226,0,150 + 226 + 300,300},
	{150 + 226,0,150 + 226 + 300,300},
	{0,0,static_cast<long>(kWallWidth),static_cast<long>(kWallHeight)},
	{0,0,static_cast<long>(kWallWidth),static_cast<long>(kWallHeight)},
};


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
	object_status_[ ObjectNum::kRogo ].texture = TextureLoder::getInstance()->load( L"Texture/title.png" );
	object_status_[ ObjectNum::kRogo ].position = kObjectposition[ ObjectNum::kRogo ];;
	object_status_[ ObjectNum::kRogo ].trim = kObjectTrim[ ObjectNum::kRogo ];
	//カーソル
	object_status_[ ObjectNum::kCusur ].texture = TextureLoder::getInstance()->load( L"Texture/title.png" );
	object_status_[ ObjectNum::kCusur ].position = kObjectposition[ ObjectNum::kCusur ];
	object_status_[ ObjectNum::kCusur ].trim = kObjectTrim[ ObjectNum::kCusur ];

	//初期の星１
	object_status_[ ObjectNum::kStar1 ].texture = TextureLoder::getInstance()->load( L"Texture/star.png" );
	object_status_[ ObjectNum::kStar1 ].position = kObjectposition[ ObjectNum::kStar1 ];
	object_status_[ ObjectNum::kStar1 ].trim = kObjectTrim[ ObjectNum::kStar1 ];

	//初期の星2
	object_status_[ ObjectNum::kStar2 ].texture = TextureLoder::getInstance()->load( L"Texture/star.png" );
	object_status_[ ObjectNum::kStar2 ].position = kObjectposition[ ObjectNum::kStar2 ];
	object_status_[ ObjectNum::kStar2 ].trim = kObjectTrim[ ObjectNum::kStar2 ];

	//初期の星3
	object_status_[ ObjectNum::kStar3 ].texture = TextureLoder::getInstance()->load( L"Texture/star.png" );
	object_status_[ ObjectNum::kStar3 ].position = kObjectposition[ ObjectNum::kStar3 ];
	object_status_[ ObjectNum::kStar3 ].trim = kObjectTrim[ ObjectNum::kStar3 ];

	//プレイヤー
	//マジックナンバー
	CsvLoader file( L"State/player_state.csv" );
	object_status_[ ObjectNum::kPlayer ].texture = TextureLoder::getInstance()->load( L"Texture/character.png" );
	object_status_[ ObjectNum::kPlayer ].position = kObjectposition[ ObjectNum::kPlayer ];
	object_status_[ ObjectNum::kPlayer ].trim = kObjectTrim[ ObjectNum::kPlayer ];

	//壁( 右 )
	object_status_[ ObjectNum::kWallRight ].texture = TextureLoder::getInstance()->load( L"Texture/wall.png" );
	object_status_[ ObjectNum::kWallRight ].position = kObjectposition[ ObjectNum::kWallRight ];
	object_status_[ ObjectNum::kWallRight ].trim = kObjectTrim[ ObjectNum::kWallRight ];

	//壁( 左 )
	object_status_[ ObjectNum::kWallLeft ].texture = TextureLoder::getInstance()->load( L"Texture/wall.png" );
	object_status_[ ObjectNum::kWallLeft ].position = kObjectposition[ ObjectNum::kWallLeft ];
	object_status_[ ObjectNum::kWallLeft ].trim = kObjectTrim[ ObjectNum::kWallLeft ];

	overlay_texture_ = TextureLoder::getInstance()->load( L"Texture/star1.png" );

	for( int i = 0; i < ObjectNum::kObjectNum; i++ )
		object_[ i ].get()->init( &object_status_[ i ] );

	volume_ = 1.0F;

	//サウンドの生成
	SOUND->play( SoundId::kTitle , true );
	return true;
}

/*===========================================================================*/
// 終了処理
void Title::destroy()
{
	for( int i = 0; i < ObjectNum::kObjectNum; i++ )
		TextureLoder::getInstance()->release( object_status_[ i ].texture );

	TextureLoder::getInstance()->release( overlay_texture_ );
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

	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	for( int i = ObjectNum::kStar1; i <= ObjectNum::kStar3; i++ )
	{
		Sprite::getInstance()->draw( overlay_texture_ , object_status_[ i ].position , &object_status_[ i ].trim , 1.0F , 1.0F );
	}
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();

	Sprite::getInstance()->draw( object_status_[ ObjectNum::kStar1 ].texture , object_status_[ ObjectNum::kStar1 ].position , &object_status_[ ObjectNum::kStar1 ].trim , 1.0F , 1.0F );
	Sprite::getInstance()->draw( object_status_[ ObjectNum::kStar2 ].texture , object_status_[ ObjectNum::kStar2 ].position , &object_status_[ ObjectNum::kStar2 ].trim , 1.0F , 1.0F);
	Sprite::getInstance()->draw( object_status_[ ObjectNum::kStar3 ].texture , object_status_[ ObjectNum::kStar3 ].position , &object_status_[ ObjectNum::kStar3 ].trim , 1.0F , 1.0F );

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
			select_menu_ = Menu::kRanking;
			SOUND->setPitch( SoundId::kSelect , 0.0F );
			SOUND->stop( SoundId::kSelect );
			SOUND->play( SoundId::kSelect , false );
		}
		else
		{
			SOUND->setPitch( SoundId::kSelect , -0.5F );
			SOUND->stop( SoundId::kSelect );
			SOUND->play( SoundId::kSelect , false );
		}
	}
	else if( key_.pressed.Up || pad_.leftStickUp == pad_.PRESSED )
	{
		if( select_menu_ == Menu::kRanking )
		{
			select_menu_ = Menu::kPlay;
			SOUND->setPitch( SoundId::kSelect , 0.0F );
			SOUND->stop( SoundId::kSelect );
			SOUND->play( SoundId::kSelect , false );
		}
		else
		{
			SOUND->setPitch( SoundId::kSelect , -0.5F );
			SOUND->stop( SoundId::kSelect );
			SOUND->play( SoundId::kSelect , false );
		}

	}
}

SceneBase* Title::playScene()
{
	volume_ -= 0.01F;
	if( volume_ < 0.0F )volume_ = 0.0F;
	SOUND->setVolume( SoundId::kScene , volume_ );
	SOUND->setVolume( SoundId::kTitle , volume_ );
	SOUND->play( SoundId::kScene , false );

	for( int i = 0; i <= ObjectNum::kStar3; i++ )
		object_status_[ i ].position.y += kFall;

	float now_time_ = object_status_[ ObjectNum::kRogo ].position.y / getWindowHeight<float>();
	if( now_time_ >= 1.0F )
		now_time_ = 1.0F;
	object_status_[ kWallRight ].position.x = getWindowWidth<float>() - Easing::getInstance()->expo( getWindowWidth<float>() / 4.0F + kWallWidth / 2.0F , now_time_ , Easing::Mode::In );
	object_status_[ kWallLeft ].position.x = -kWallWidth + Easing::getInstance()->expo( 320.0F + kWallWidth / 2.0F, now_time_ , Easing::Mode::In );

	if( object_status_[ kRogo ].position.y > getWindowHeight<float>() )
	{
		SOUND->stop( SoundId::kScene );
		SOUND->stop( SoundId::kTitle );
		SOUND->setVolume( SoundId::kTitle , 1.0F );
		return new Endless;
	}
	return this;
}

SceneBase * Title::rankingScene()
{
	volume_ -= 0.01F;
	SOUND->setVolume( SoundId::kScene , volume_ );
	SOUND->play( SoundId::kScene , false );
	for( int i = 0; i < ObjectNum::kObjectNum; i++ )
		object_status_[ i ].alpha = volume_;


	if( volume_ < 0 )
	{
		SOUND->stop( SoundId::kScene );
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

	if( key_.released.Space ||
		pad_.a == pad_.RELEASED )
	{
		SOUND->stop( SoundId::kDicision );
		SOUND->play( SoundId::kDicision , false );
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
		object_status_[ ObjectNum::kCusur ].position.y = kObjectposition[ ObjectNum::kCusur ].y + kCusurInterval * select_menu_;

	return this;
}
