#include "stay_particle.h"
#include "sprite.h"
#include "textureLoder.h"
#include "task_manager.h"

const int kRange = 8;
const int kTextureSize = 32;


StayParticle::StayParticle( TaskManager* Manager ):
ObjectBase(Manager)
{
	task_manager_ = Manager;
}


StayParticle::~StayParticle()
{}

bool StayParticle::init( const std::wstring & FileName , Vector2* Posit )
{
	texture_ = TextureLoder::getInstance()->load( FileName );
	angle_ = static_cast< float >( rand() % 360 );
	length_ = static_cast< float >( rand() % kRange );
	alpha_ = ( rand() % 10 + 1 ) / 10.0F;
	position_ = Posit;

	task_manager_->registerTask( this , TaskUpdate::kParticleUpdate );
	task_manager_->registerTask( this , TaskDraw::kDraw );
	return true;
}

void StayParticle::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
	task_manager_->unregisterObject( this );
}

void StayParticle::update()
{
		alpha_ -= 0.1F;
}

void StayParticle::draw()
{
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	Sprite::getInstance()->draw( texture_ ,
		( *position_ ) + Vector2( std::cos( XMConvertToRadians( angle_ ) ) , -std::sin( XMConvertToRadians( angle_ ) ) ) * length_ ,
								 nullptr ,
								 alpha_ ,
								 0.9F ,
								 Vector2( 1.0F , 1.0F ) ,
								 0.0F ,
								 Vector2( kTextureSize / 2.0F , kTextureSize / 2.0F ) );
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();

}

bool StayParticle::isAlive()
{
	if( alpha_ <= 0.0F )
		return false;

	return true;
}
