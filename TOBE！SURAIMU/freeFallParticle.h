#pragma once
#include "object_base.h"

class FreeFallParticle :
	public ObjectBase
{
public:
	FreeFallParticle();
	~FreeFallParticle();

public:
	bool init( const std::wstring& FileName , const Vector2& Posit , const float MoveAmount );
	void destroy() override;
	void update() override;
	void draw() override;
	bool isAlive() override;

private:
	float alpha_;
	float angle_;
	float move_amount_;
	int rotate_;
	bool turn_;
};

