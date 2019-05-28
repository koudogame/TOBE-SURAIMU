#include "freeFallParticle.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"

const float kMinFall = 0.0F;
const int kTextureSize = 64;
const int kRotate = 5;

FreeFallParticle::FreeFallParticle()
{}


FreeFallParticle::~FreeFallParticle()
{}

bool FreeFallParticle::init(const Vector2& Posit , const RECT& Triming )
{
	texture_ = TextureLoder::getInstance()->load( L"Texture/Particle.png" );
	if( texture_ == nullptr )
		return false;

	TaskManager::getInstance()->registerTask( this , TaskUpdate::kParticleUpdate );
	TaskManager::getInstance()->registerTask( this , TaskDraw::kDraw );

	position_ = Posit;
	alpha_ = 1.0F;
	turn_ = rand() % 2 ? true : false;
	rotate_ = rand() % 72;
	triming_ = Triming;

	return true;
}

void FreeFallParticle::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
	TaskManager::getInstance()->unregisterObject( this );
}

void FreeFallParticle::update()
{
	alpha_ -= 0.01F;
	rotate_ += turn_ ? kRotate : -kRotate;
}

void FreeFallParticle::draw()
{
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	Sprite::getInstance()->draw( texture_ , position_ , &triming_ , alpha_ , 0.2F , Vector2( 1.0F , 1.0F ) , static_cast< float >( rotate_ ) , Vector2( kTextureSize / 2.0F , kTextureSize / 2.0F ) );
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();
}

bool FreeFallParticle::isAlive()
{
	if( alpha_ <= 0.F )
		return false;

	return true;
}
