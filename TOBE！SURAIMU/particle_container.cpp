
// 板場 温樹

#include "particle_containerr.h"


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
	const wchar_t* const TextureFileName,
	const Vector2& Position, const Vector2& Velocity,
	const Timer<Seconds>& Clock, const long long LifeTimeSec)
{
	Particle* particle = getFreeObjAndInsert();
	if (particle == nullptr) { return nullptr; }

	particle->init(
		TextureFileName, 
		Position,
		Velocity,
		Clock,
		LifeTimeSec
	);

	return particle;
}
