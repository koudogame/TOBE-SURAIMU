#pragma once

#include "object_container_base.h"

class Particle;
class TaskManager;

class ParticleContainer :
	public ObjectContainerBase<Particle>
{
public:
	ParticleContainer(TaskManager* const TaskManager);
	virtual ~ParticleContainer();
public:
	Particle* addParticle(
		const wchar_t* const TextureFileName,
		const Vector2& Velocity,
		const long long LifeTimeMs
	);
};
