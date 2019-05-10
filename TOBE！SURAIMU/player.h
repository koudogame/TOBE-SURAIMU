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
	//継承関数
	bool init(const Vector2& Posit,const float Jump = 30.0F,const float Decay = 0.5F,const float Gravity = 0.5F,const float Speed = 5.0F,const float Boost = 10.0F);
	void destroy();
	void update();
	void draw();
	bool isLife();

public:
	//外部利用関数
	inline const Circle& getShape() { return myshape_; }
	inline const Line& getMove() { return move_vector_; }
	void revision( const Vector2& CrossPoint );


private:
	//定数化変数
	float kJumpPower;
	float kDecay;
	float kGravity;
	float kSpeed;
	float kBoostPower;
	Line kGround;

private:
	//メンバ変数
	Line move_vector_;

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
	Circle myshape_;

private:
	//内部利用関数
	void input();
	void gravity();
	void setGravityAngle();

};

