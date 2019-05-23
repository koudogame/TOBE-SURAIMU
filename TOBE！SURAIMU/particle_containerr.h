#pragma once

// ”Âê ‰·÷

#include "object_container_base.h"
#include "particle.h"

class ParticleContainer :
	public ObjectContainerBase<Particle>
{
public:
	ParticleContainer();
	virtual ~ParticleContainer();
public:
	Particle* addParticle(
		const wchar_t* const TextureFileName,
		const Vector2& Position,
		const Vector2& Velocity,
		const Timer<Milliseconds>& Clock,
		const long long LifeTimeMs
	);
};
