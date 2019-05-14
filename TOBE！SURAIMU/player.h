#pragma once
#include "object_base.h"
#include "shape.h"
#include "numbers.h"

class TaskManager;

class Player :
	public ObjectBase
{
public:
	Player( TaskManager* const Manager );
	~Player();

public:
	//継承関数
	bool init(const Vector2& Posit,const float Jump = 30.0F,const float Decay = 0.5F,const float Gravity = 0.5F,const float Speed = 5.0F , const float UpBoost = 10.0F , const float RLBoost = 0.0F );
	void destroy();
	void update();
	void draw();
	bool isLife();

public:
	//外部利用関数
	inline Circle* getShape() { return &myshape_; }
	inline Line* getMove() { return &move_vector_; }
	inline void setGround( Line* const Ground ) { ground_ = Ground; }
	inline bool isCollision() { return flag_.test( Flag::kCollision ); }
	inline bool isJump() { return flag_.test( Flag::kJump ); }
	inline ObjectBase* getOwner() { return owner_; }
	void revision( const Vector2& CrossPoint );
	void collision( class Star* );
	void collision( class Wall* );
	float getRotate();


private:
	//定数化変数
	float kJumpPower;
	float kDecay;
	float kGravity;
	float kSpeed;
	float kUPBoostPower;
	float kRLBoostPower;
	Line kGround;

private:
	//メンバ変数
	Line move_vector_;

	enum Flag
	{
		kJump,
		kBoost,
		kCollision
	};
	std::bitset<3> flag_;
	float jump_power_;
	float boost_power_;
	float gravity_angle_;
	float jumping_angle_;
	float ditrection_angle_;
	Line* ground_;
	ObjectBase* owner_;
	Circle myshape_;
	float dis_;

	Numbers<long> num;
	ID3D11ShaderResourceView* Num;

private:
	//内部利用関数
	void input();
	void gravity();
	void setGravityAngle();

};