#pragma once
#include "object_base.h"
#include "shape.h"

class TaskManager;

class Player :
	public ObjectBase
{
public:
	Player( TaskManager* const Manager );
	~Player();

public:
	//Œp³ŠÖ”
	bool init(const Vector2& Posit,const float Jump = 30.0F,const float Decay = 0.5F,const float Gravity = 0.5F,const float Speed = 5.0F,const float Boost = 10.0F);
	void destroy();
	void update();
	void draw();
	bool isLife();


private:
	//’è”‰»•Ï”
	float kJumpPower;
	float kDecay;
	float kGravity;
	float kSpeed;
	float kBoostPower;
	Line kGround;

private:
	//ƒƒ“ƒo•Ï”
	Line move_vector;

	enum Flag
	{
		kJump,
		kBoost
	};
	std::bitset<2> flag_;
	float jump_power_;
	float boost_power_;
	Vector2 movement_;
	float gravity_angle_;
	float jumping_angle_;
	float ditrection_angle_;
	Line* ground_;
	ObjectBase* owner_;

private:
	//“à•”—˜—pŠÖ”
	void input();
	void gravity();
	void setGravityAngle();

};

