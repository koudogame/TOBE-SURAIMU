#include "stay_particle.h"
#include "sprite.h"
#include "textureLoder.h"
#include "task_manager.h"

const int kRtate = 4;			//‰ñ“]Šp
const int kTextureSize = 46;	//ƒeƒNƒXƒ`ƒƒƒTƒCƒY


StayParticle::StayParticle()
{}


StayParticle::~StayParticle()
{}

//‰Šú‰»
bool StayParticle::init( Vector2* Posit , const int TurnDirection )
{
	//ƒeƒNƒXƒ`ƒƒ‚Ìæ“¾
	texture_ = TextureLoder::getInstance()->load( L"Texture/character.png" );
	turn_ = -1;
	if( TurnDirection )
		turn_ = 1;
	position_ = Posit;

	//ƒ^ƒXƒN‚Ö‚Ì“o˜^
	TaskManager::getInstance()->registerTask( this , TaskUpdate::kParticleUpdate );
	TaskManager::getInstance()->registerTask( this , TaskDraw::kParticle );
	return true;
}

//”jŠü
void StayParticle::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
	TaskManager::getInstance()->unregisterObject( this );
}

//XV
void StayParticle::update()
{
	angle_ += turn_ * kRtate;
}

//•`‰æ
void StayParticle::draw()
{
	//Ø‚èæ‚èˆÊ’u
	RECT trim;
	trim.left = kTextureSize * 2;
	trim.top = 0;
	trim.right = trim.left + kTextureSize;
	trim.bottom = trim.top + kTextureSize;
	//‰ÁZ‡¬
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	Sprite::getInstance()->draw( texture_ ,
								( *position_ ) ,
								 &trim ,
								 1.0F ,
								 0.9F ,
								 Vector2( 1.0F , 1.0F ) ,
								 angle_ ,
								 Vector2( kTextureSize / 2.0F , kTextureSize / 2.0F ) );
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();

}