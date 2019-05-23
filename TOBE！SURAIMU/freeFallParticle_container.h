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
	FreeFallParticle* addParticle( const std::wstring& FileName ,
								   const Vector2& Position,
								   const float MoveAmound);
	void setMove( const float Over );
};

