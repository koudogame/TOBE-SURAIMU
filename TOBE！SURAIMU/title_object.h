#pragma once
#include "object_base.h"

class TitleObject
{
public:
	TitleObject();
	~TitleObject();

	//åpè≥ä÷êî
public:
	bool init( const Vector2& Posit , const RECT& Triming );
	void destroy();
	void update();
	void draw(const float Alpha);
	bool isAlive();

public:
	inline void setPosition( const Vector2& Posit ) { position_ = Posit; }

private:
	RECT trim_;
	Vector2 position_;
	ID3D11ShaderResourceView* texture_;
};

