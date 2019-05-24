#pragma once
#include "object_base.h"

class FreeFallParticle :
	public ObjectBase
{
public:
	FreeFallParticle();
	~FreeFallParticle();

public:
	bool init(const Vector2& Posit ,const RECT& Triming);
	void destroy() override;
	void update() override;
	void draw() override;
	bool isAlive() override;

private:
	RECT triming_;
	float alpha_;
	float angle_;
	float move_amount_;
	int rotate_;
	bool turn_;
};

