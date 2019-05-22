#include "stdafx.h"
#include "stay_particle_container.h"


StayParticleContainer::StayParticleContainer( TaskManager* const Manager , Vector2* Posit ):
ObjectContainerBase(Manager)
{
	task_manager_ = Manager;
	position_ = Posit;
}


StayParticleContainer::~StayParticleContainer()
{}

StayParticle * StayParticleContainer::addParticle( const std::wstring & FileName )
{
	//‹óƒRƒ“ƒeƒi‚ðŽæ“¾
	StayParticle* s_particle = getFreeObjAndInsert();
	if( s_particle == nullptr )
		return nullptr;

	//‰Šú‰»
	s_particle->init( FileName , position_ );
	return s_particle;
}
