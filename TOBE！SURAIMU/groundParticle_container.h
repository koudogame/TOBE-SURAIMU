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
	enum ParticleID
	{
		kCyan ,
		kYellow ,
		kMaggenta ,
		kWall,
		kNonParticle
	};

public:
	GroundParticle* addParticle(
		const Vector2 Position ,
		const float Angle,
		ParticleID ID );

	void setMove( const float Over );
};

