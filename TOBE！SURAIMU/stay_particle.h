#pragma once
//*******************************************************************
//CreatorFR“c@°‹v
#include "object_base.h"

class StayParticle :
	public ObjectBase
{
public:
	StayParticle();
	~StayParticle();

public:
	//‰Šú‰»
	//ˆø”...•`‰æÀ•W:‰ñ“]•ûŒü
	bool init( Vector2* Posit , const int TurnDirection );
	//”jŠü
	void destroy() override;
	//XV
	void update() override;
	//•`‰æ
	void draw() override;

private:
	int	turn_;		    //‰ñ“]•ûŒü
	float angle_;	    //Šp“x
	Vector2* position_;	//À•W
};

