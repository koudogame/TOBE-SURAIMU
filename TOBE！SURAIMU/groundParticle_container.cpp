#include "groundParticle_container.h"
#include "textureLoder.h"


GroundParticle_container::GroundParticle_container( TaskManager * TaskManager ) :
	ObjectContainerBase( TaskManager )
{
	task_manager_ = TaskManager;
}

GroundParticle_container::~GroundParticle_container()
{}

GroundParticle * GroundParticle_container::addParticle( const wchar_t * const TextureFileName , const Vector2 & Position , const float Angle )
{
	GroundParticle* particle = getFreeObjAndInsert();
	if( particle == nullptr ) { return nullptr; }

	particle->init(
		TextureFileName ,
		Position ,
		Angle
	);

	return particle;
}
