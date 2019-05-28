#pragma once
#include "object_container_base.h"
#include "groundParticle.h"

class GroundParticleContainer :
	public ObjectContainerBase<GroundParticle>
{
public:
	GroundParticleContainer();
	~GroundParticleContainer();

public:
	GroundParticle* addParticle(
		const Vector2 Position ,
		const float Angle,
		NameSpaceParticle::ParticleID ID,
		const float Scale = 1.0F );

	void setMove( const float Over );
};

