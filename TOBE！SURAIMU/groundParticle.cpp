#include "groundParticle.h"
#include "textureLoder.h"
#include "easing.h"
#include "sprite.h"
#include "task_manager.h"

const int kTextureSize = 32;

GroundParticle::GroundParticle( TaskManager * Manager ) :
	ObjectBase( ObjectID::kGroundParticle , Manager )
{
	task_manager_ = Manager;
}

GroundParticle::~GroundParticle()
{}

bool GroundParticle::init(const std::wstring& FileName, Vector2 Posit , const float Angle )
{
	task_manager_->registerTask( this , TaskUpdate::kParticleUpdate );
	task_manager_->registerTask( this , TaskDraw::kParticleDraw );
	texture_ = TextureLoder::getInstance()->load( FileName );
	if( texture_ == nullptr )
		return false;

	position_ = Posit;
	angle_ = Angle;
	now_time_ = 0;

	return true;
}

void GroundParticle::destroy()
{
	task_manager_->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

void GroundParticle::update()
{
 	velocity_ = Vector2( std::cos( angle_ ) , -std::sin( angle_ ) ) * Easing::getInstance()->expo( 100.0F , now_time_ , Easing::Mode::Out );
	now_time_ += 0.01F;
}

void GroundParticle::draw()
{
	Sprite::getInstance()->draw( texture_ , position_ + velocity_ , nullptr , 1.0F - now_time_ , 0.0F , Vector2( 0.5F , 0.5F ) , 0.0F , Vector2( kTextureSize / 2.0F , kTextureSize / 2.0F ) );
}

bool GroundParticle::isAlive()
{
	if( now_time_ >= 1.0F )
		return false;

	return true;
}
