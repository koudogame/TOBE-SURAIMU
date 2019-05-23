#pragma once
#include "object_base.h"

class TitleObject :
	public ObjectBase
{
public:
	TitleObject();
	~TitleObject();

	//åpè≥ä÷êî
public:
	bool init( const Vector2& Posit , const RECT& Triming );
	void destroy()override;
	void update()override;
	void draw()override;
	bool isAlive()override;

public:
	inline void setPosition( const Vector2& Posit ) { position_ = Posit; }

private:
	RECT trim_;
};

