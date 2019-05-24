#include "stay_particle.h"
#include "sprite.h"
#include "textureLoder.h"
#include "task_manager.h"

const int kRtate = 4;
const int kTextureSize = 46;


StayParticle::StayParticle()
{}


StayParticle::~StayParticle()
{}

bool StayParticle::init( Vector2* Posit , const int TurnDirection )
{
	texture_ = TextureLoder::getInstance()->load( L"Texture/character.png" );
	turn_ = -1;
	if( TurnDirection )
		turn_ = 1;
	position_ = Posit;

	TaskManager::getInstance()->registerTask( this , TaskUpdate::kParticleUpdate );
	TaskManager::getInstance()->registerTask( this , TaskDraw::kDraw );
	return true;
}

void StayParticle::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
	TaskManager::getInstance()->unregisterObject( this );
}

void StayParticle::update()
{
	angle_ += turn_ * kRtate;
}

void StayParticle::draw()
{
	RECT trim;
	trim.left = kTextureSize * 2;
	trim.top = 0;
	trim.right = trim.left + kTextureSize;
	trim.bottom = trim.top + kTextureSize;
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	Sprite::getInstance()->draw( texture_ ,
								( *position_ ) ,
								 &trim ,
								 1.0 ,
								 0.9F ,
								 Vector2( 1.0F , 1.0F ) ,
								 angle_ ,
								 Vector2( kTextureSize / 2.0F , kTextureSize / 2.0F ) );
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();

}