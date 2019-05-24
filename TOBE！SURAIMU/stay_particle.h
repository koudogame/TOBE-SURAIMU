#pragma once
#include "object_base.h"

class StayParticle :
	public ObjectBase
{
public:
	StayParticle();
	~StayParticle();

public:
	bool init( Vector2* Posit , const int TurnDirection );
	void destroy() override;
	void update() override;
	void draw() override;

private:
	int turn_;
	float angle_;
	Vector2* position_;
};

