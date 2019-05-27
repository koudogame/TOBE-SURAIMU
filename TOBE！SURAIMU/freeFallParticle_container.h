#pragma once
#include "object_container_base.h"
#include "freeFallParticle.h"

class FreeFallParticleContainer :
	public ObjectContainerBase<FreeFallParticle>
{
public:
	FreeFallParticleContainer();
	~FreeFallParticleContainer();

public:
	FreeFallParticle* addParticle( const Vector2& Position ,
								   NameSpaceParticle::ParticleID ID );
	void setMove( const float Over );
};

