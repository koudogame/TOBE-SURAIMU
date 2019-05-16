#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "particle.h"

class GroundParticle :
	public Particle
{
public:
	GroundParticle( TaskManager* const TaskManager );
	~GroundParticle();

public:
	bool init(
		const wchar_t* const TextureFileName ,
		const Vector2& Position,
		const float Angle);
	void update() override;
	void draw() override;

private:
	float now_time_;
	float angle_;
};