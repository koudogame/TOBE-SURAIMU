#include "freeFallParticle_container.h"

const int kTextureSize = 64;

FreeFallParticleContainer::FreeFallParticleContainer()
{}


FreeFallParticleContainer::~FreeFallParticleContainer()
{}

FreeFallParticle * FreeFallParticleContainer::addParticle( const Vector2 & Position , const float MoveAmound , ParticleID ID )
{
	//‹óƒRƒ“ƒeƒi‚ðŽæ“¾
	FreeFallParticle* s_particle = getFreeObjAndInsert();
	if( s_particle == nullptr )
		return nullptr;

	RECT trim = { 0,0,kTextureSize,kTextureSize };
	trim.left += kTextureSize * ID;
	trim.right = trim.left + kTextureSize;

	//‰Šú‰»
	s_particle->init( Position , trim , MoveAmound );
	return s_particle;
}

void FreeFallParticleContainer::setMove( const float Over )
{
	std::list<FreeFallParticle*> list = this->active();

	for( auto& itr : list )
		itr->setMove( Over );
}
