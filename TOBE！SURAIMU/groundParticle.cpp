#include "groundParticle.h"
#include "textureLoder.h"
#include "easing.h"
#include "sprite.h"
#include "task_manager.h"

const int kTextureSize = 64;

GroundParticle::GroundParticle()
{}

GroundParticle::~GroundParticle()
{}

bool GroundParticle::init( const Vector2 & Posit , const RECT & Triming , const float Angle )
{
	TaskManager::getInstance()->registerTask( this , TaskUpdate::kParticleUpdate );
	TaskManager::getInstance()->registerTask( this , TaskDraw::kParticle );
	texture_ = TextureLoder::getInstance()->load( L"Texture/Particle.png" );
	if( texture_ == nullptr )
		return false;

	position_ = Posit;
	angle_ = Angle;
	now_time_ = 0;
	triming_ = Triming;

	return true;
}

void GroundParticle::destroy()
{
	TaskManager::getInstance()->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

void GroundParticle::update()
{
 	velocity_ = Vector2( std::cos( angle_ ) , -std::sin( angle_ ) ) * Easing::getInstance()->expo( 100.0F , now_time_ , Easing::Mode::Out );
	now_time_ += 0.02F;
}

void GroundParticle::draw()
{
	Sprite::getInstance()->draw( texture_ , position_ + velocity_ , &triming_ , 1.0F - now_time_ , 0.2F , Vector2(1.0F , 1.0F ) , 0.0F , Vector2( kTextureSize / 2.0F , kTextureSize / 2.0F ) );
}

bool GroundParticle::isAlive()
{
	if( now_time_ >= 1.0F )
		return false;

	return true;
}
