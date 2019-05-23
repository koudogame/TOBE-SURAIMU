#include "freeFallParticle_container.h"


FreeFallParticleContainer::FreeFallParticleContainer()
{}


FreeFallParticleContainer::~FreeFallParticleContainer()
{}

FreeFallParticle * FreeFallParticleContainer::addParticle( const std::wstring & FileName , const Vector2 & Position, const float MoveAmound )
{
	//‹óƒRƒ“ƒeƒi‚ðŽæ“¾
	FreeFallParticle* s_particle = getFreeObjAndInsert();
	if( s_particle == nullptr )
		return nullptr;

	//‰Šú‰»
	s_particle->init( FileName , Position , MoveAmound );
	return s_particle;
}

void FreeFallParticleContainer::setMove( const float Over )
{
	std::list<FreeFallParticle*> list = this->active();

	for( auto& itr : list )
		itr->setMove( Over );
}
