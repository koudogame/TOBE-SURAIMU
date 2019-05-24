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
	enum ParticleID
	{
		kCyan ,
		kYellow ,
		kMagenta ,
		kPlayer = 4,
	};

public:
	FreeFallParticle* addParticle( const Vector2& Position ,
								   ParticleID ID );
	void setMove( const float Over );
};

