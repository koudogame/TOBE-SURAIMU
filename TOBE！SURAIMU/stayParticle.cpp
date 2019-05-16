#include "stayParticle.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"


StayParticle::StayParticle( TaskManager* Manager ) :
	ObjectBase( ObjectID::kStayParticle , Manager )
{
	task_manager_ = Manager;
}


StayParticle::~StayParticle()
{}

bool StayParticle::init( const std::wstring& FileName , const Vector2& Posit )
{
	texture_ = TextureLoder::getInstance()->load( FileName );
	if( texture_ == nullptr )
		return false;

	task_manager_->registerTask( this , TaskUpdate::kParticleUpdate );
	task_manager_->registerTask( this , TaskDraw::kParticleDraw );

	position_ = Posit;
	alpha_ = 1.0F;
	return false;
}

void StayParticle::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
	task_manager_->unregisterObject( this );
}

void StayParticle::update()
{
	alpha_ -= 0.01F;
}

void StayParticle::draw()
{
	Sprite::getInstance()->draw( texture_ , position_ , nullptr , alpha_ );
}

bool StayParticle::isAlive()
{
	if( alpha_ <= 0.F )
		return false;

	return true;
}
