#pragma once
#include "object_container_base.h"
#include "stay_particle.h"

class StayParticleContainer :
	public ObjectContainerBase<StayParticle>
{
public:
	StayParticleContainer(Vector2* Posit );
	~StayParticleContainer();

public:
	StayParticle* addParticle( const int TurnDirection );

private:
	Vector2* position_;
};

