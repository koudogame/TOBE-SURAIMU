#include "stdafx.h"
#include "stay_particle_container.h"


StayParticleContainer::StayParticleContainer( Vector2* Posit )
{
	position_ = Posit;
}


StayParticleContainer::~StayParticleContainer()
{}

StayParticle * StayParticleContainer::addParticle( const int TurnDirection )
{
	//‹óƒRƒ“ƒeƒi‚ðŽæ“¾
	StayParticle* s_particle = getFreeObjAndInsert();
	if( s_particle == nullptr )
		return nullptr;

	//‰Šú‰»
	s_particle->init( position_ , TurnDirection );
	return s_particle;
}
