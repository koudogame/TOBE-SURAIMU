#pragma once
//========================================
//CreatorName:YamadaHaruhisa

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
	//Œp³ŠÖ”
	bool init(const Vector2& Posit,const float Jump = 30.0F,const float AddVol = 0.001F,const float Decay = 0.5F,const float Gravity = 0.5F,const float Speed = 5.0F , const float UpBoost = 10.0F , const float RLBoost = 0.0F );
	void destroy();
	void update();
	void draw();
	bool isAlive();
	void setPosition( const Vector2& Dist ) { myshape_.position = Dist; }

public:
	//ŠO•”—˜—pŠÖ”
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
	//’è”‰»•Ï”
	float kJumpAmount;
	float kAddVolume;
	float kDecay;
	float kGravity;
	float kSpeed;
	float kUPBoostPower;
	float kRLBoostPower;
	Line kGround;

private:
	//ˆ—Œn
	//ƒƒ“ƒo•Ï”
	Line move_vector_;

	enum Flag
	{
		kJump,
		kBoost,
		kCollision
	};
	std::bitset<3> flag_;
	float now_amount_;
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
	//•`‰æŒn
	enum Direction
	{
		kFlont,
		kSquat,
		kFlay
	};

	Direction direction_id_;
	Vector2 rect_left_up_;
	void slectDirection();

	int timer;

private:
	//“à•”—˜—pŠÖ”
	void input();
	void gravity();
	void setGravityAngle();
};