#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "object_base.h"
#include "shape.h"
#include "groundParticle_container.h"
#include "freeFallParticle_container.h"
#include "stay_particle_container.h"
#include "scoring.h"

#include "audio_loader.h"


class Player :
	public ObjectBase
{
public:
	Player();
	~Player();

public:
	//継承関数
	bool init(const Vector2& Posit,const float Jump = 30.0F,const float AddVol = 0.001F,const float Decay = 0.5F,const float Gravity = 0.5F,const float Speed = 5.0F , const float RLBoost = 0.0F );
	void destroy();
	void update();
	void draw();
	bool isAlive();
	void setMove( const float Over );
	Vector2 getPosition() const { return myshape_.position; }

public:
	//外部利用関数
	inline Circle* getShape() { return &myshape_; }
	inline Line* getMove() { return &move_vector_; }
	inline void setGround( Line* const Ground ) { ground_ = Ground; }
	inline bool isCollision() { return flag_.test( Flag::kStarCollision ); }
	inline bool isJump() { return flag_.test( Flag::kJump ); }
	inline ObjectBase* getOwner() { return owner_; }
	void revision( const Vector2& CrossPoint, NameSpaceParticle::ParticleID ID );
	void collision( class Star* );
	void collision( class Wall* );
	float getRotate();
	void resetStatus( const float Magnification ) { magnification_ = Magnification; }
	inline void onStartFlag() { score_.start(); }
	inline void addScore( const float AddScore ) { score_.addDefaultScore( AddScore ); }
	inline Scoring* getScore() { return &score_; }


private:
	//定数化変数
	float kJumpAmount;
	float kAddVolume;
	float kDecay;
	float kGravity;
	float kSpeed;
	float kRLBoostPower;
	Line kGround;

private:
	//処理系
	//メンバ変数
	Line move_vector_;

	enum Flag
	{
		kJump,
		kBoost,
		kStarCollision,
		kParticle,
		kTechnique,
		kWallParticle,
	};
	std::bitset<6> flag_;
	float now_amount_;
	float boost_power_;
	float gravity_angle_;
	float jumping_angle_;
	float ditrection_angle_;
	Line* ground_;
	ObjectBase* owner_;
	Circle myshape_;
	float dis_;
	std::unique_ptr<GroundParticleContainer> g_particle_container_;
	std::unique_ptr<FreeFallParticleContainer> f_particle_container_;
	std::unique_ptr<StayParticleContainer> s_particle_container_;
	int particle_time_;
	float prev_jump_moveamount_;
	float magnification_;
	int bottom_input_;
	bool died_flag_;
	float collision_combo_pitch_;
	ID3D11ShaderResourceView* guide_;
	float guide_alpha_;

	Scoring score_;

	AudioContainer* sound_[ 2 ];
	AudioContainer* died_sound_;

private:
	//描画系
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
	//内部利用関数
	void input();
	void gravity();
	void setGravityAngle();
	//**注意**
	//プレイヤーの座標が交点の位置で
	//重力方向が更新された後に呼び出すこと
	void addGroundParticle( NameSpaceParticle::ParticleID ID );

	void addFreeFallParticle();
	bool diedEffect();
};