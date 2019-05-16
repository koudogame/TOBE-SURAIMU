#pragma once
#include "object_container_base.h"
#include "groundParticle.h"

class GroundParticleContainer :
	public ObjectContainerBase<GroundParticle>
{
public:
	GroundParticleContainer();
	~GroundParticleContainer();
};

