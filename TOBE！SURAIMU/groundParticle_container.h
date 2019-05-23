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
		const std::wstring& FileName ,
		const Vector2 Position ,
		const float Angle );

	void setMove( const float Over );
};

