#pragma once
#include "object_container_base.h"
#include "stayParticle.h"

class StayParticleContainer :
	public ObjectContainerBase<StayParticle>
{
public:
	StayParticleContainer();
	~StayParticleContainer();
};

