#include "groundParticle.h"
#include "sprite.h"
#include "easing.h"
#include "textureLoder.h"


GroundParticle::GroundParticle( TaskManager* const TaskManager):
Particle(TaskManager)
{}

GroundParticle::~GroundParticle()
{}

bool GroundParticle::init( const wchar_t * const TextureFileName , const Vector2 & Position , const float Angle )
{

	texture_ = TextureLoder::getInstance()->load( TextureFileName );
	if( texture_ == nullptr ) { return false; }

	now_time_ = 0;
	position_ = Position;
	angle_ = Angle;
	return false;
}

void GroundParticle::update()
{
	if( now_time_ >= 1.0F )
	{
		is_alive_ = false;
		return;
	}
	velocity_ = Vector2( std::cos( angle_ ) , -std::sin( angle_ ) ) * Easing::getInstance()->expo( 100 , now_time_ , Easing::Mode::Out );
	now_time_ += 0.01F;
}

void GroundParticle::draw()
{
	Sprite::getInstance()->draw(
		texture_,
		position_ + velocity_,
		nullptr
	);
}
