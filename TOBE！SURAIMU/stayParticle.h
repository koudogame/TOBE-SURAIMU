#pragma once
#include "object_base.h"

class StayParticle :
	public ObjectBase
{
public:
	StayParticle( TaskManager* Manager );
	~StayParticle();

public:
	bool init( const std::wstring& FileName , const Vector2& Posit );
	void destroy() override;
	void update() override;
	void draw() override;
	bool isAlive() override;

private:
	float alpha_;
};

