#pragma once
#include "object_base.h"

class StayParticle :
	public ObjectBase
{
public:
	StayParticle();
	~StayParticle();

public:
	bool init( const std::wstring& FileName , Vector2* Posit );
	void destroy() override;
	void update() override;
	void draw() override;
	bool isAlive() override;

private:
	float alpha_;
	float angle_;
	float length_;
	Vector2* position_;
};

