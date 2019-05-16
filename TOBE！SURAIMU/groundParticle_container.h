#pragma once
#include "object_container_base.h"
#include "groundParticle.h"

class GroundParticle_container:
	ObjectContainerBase<GroundParticle>
{
public:
	GroundParticle_container(TaskManager* TaskManager);
	~GroundParticle_container();

public:
	GroundParticle* addParticle(
		const wchar_t* const TextureFileName,
		const Vector2& Position,
		const float Angle
	);
};

