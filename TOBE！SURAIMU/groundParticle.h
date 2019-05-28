#pragma once
#include "object_base.h"
class GroundParticle :
	public ObjectBase
{
public:
	GroundParticle();
	~GroundParticle();

public:
	bool init( const Vector2& Posit , const RECT& Triming , const float Angle , const float Scale );
	void destroy() override;
	void update()override;
	void draw()override;
	bool isAlive() override;

private:
	Vector2 velocity_;
	RECT triming_;
	float angle_;
	float now_time_;
	float scale_;
};

