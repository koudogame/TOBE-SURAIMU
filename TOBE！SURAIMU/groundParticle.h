#pragma once
#include "object_base.h"
class GroundParticle :
	public ObjectBase
{
public:
	GroundParticle( TaskManager* Manager );
	~GroundParticle();

public:
	bool init( const std::wstring& FileName, Vector2 Posit,const float Angle);
	void destroy() override;
	void update()override;
	void draw()override;
	bool isAlive() override;

private:
	Vector2 velocity_;
	float angle_;
	float now_time_;
};

