#pragma once
//*******************************************************************
//CreatorFR“c@°‹v

#include "object_base.h"
class GroundParticle :
	public ObjectBase
{
public:
	GroundParticle();
	~GroundParticle();

	//Œp³ŠÖ”
public:
	//‰Šú‰»
	//ˆø”...•`‰æˆÊ’u:Ø‚èæ‚è”ÍˆÍ:ˆÚ“®‚·‚éŠp“x( rad ):Šgk—Ê
	bool init( const Vector2& Posit , const RECT& Triming , const float Angle , const float Scale );
	//”jŠü
	void destroy() override;
	//XV
	void update()override;
	//•`‰æ
	void draw()override;
	//¶‘¶Šm”F
	bool isAlive() override;

private:
	Vector2 velocity_;		//‘¬“x
	RECT triming_;			//Ø‚èæ‚è”ÍˆÍ
	float angle_;			//ˆÚ“®Šp“x
	float now_time_;		//“ü—Í—Ê
	float scale_;			//Šgk—Ê
};

