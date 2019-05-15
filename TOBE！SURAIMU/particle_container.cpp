
// 板場 温樹

#include "particle_containerr.h"

#include "particle.h"


/*===========================================================================*/
ParticleContainer::ParticleContainer(TaskManager* const TaskManager) :
	ObjectContainerBase(TaskManager)
{
}

ParticleContainer::~ParticleContainer()
{
}

/*===========================================================================*/
// パーティクルの追加
Particle* ParticleContainer::addParticle(
	const wchar_t* const TextureFileName, const Vector2& Velocity,
	const long long LifeTimeMs)
{
	Particle* particle = getFreeObjAndInsert();
	if (particle == nullptr) { return nullptr; }

	particle->init(
		TextureFileName, 
		Velocity,
		LifeTimeMs
	);

	return particle;
}
