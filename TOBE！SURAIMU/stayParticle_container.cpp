#include "stayParticle_container.h"


StayParticleContainer::StayParticleContainer( TaskManager* const Manager ) :
	ObjectContainerBase( Manager )
{}


StayParticleContainer::~StayParticleContainer()
{}

StayParticle* StayParticleContainer::addParticle( const std::wstring & FileName , const Vector2 & Position )
{
	//‹óƒRƒ“ƒeƒi‚ðŽæ“¾
	StayParticle* s_particle = getFreeObjAndInsert();
	if( s_particle == nullptr )
		return nullptr;

	//‰Šú‰»
	s_particle->init( FileName , Position );
	return s_particle;
}
