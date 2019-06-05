#pragma once
//*******************************************************************
//Creator：山田　晴久
#include "object_container_base.h"
#include "groundParticle.h"

class GroundParticleContainer :
	public ObjectContainerBase<GroundParticle>
{
public:
	GroundParticleContainer();
	~GroundParticleContainer();

	//継承用関数
public:
	//パーティクルの追加
	//引数...描画位置:移動方向( rad ):パーティクルID:拡縮量( Default 1.0 )
	GroundParticle* addParticle(
		const Vector2 Position ,
		const float Angle,
		NameSpaceParticle::ParticleID ID,
		const float Scale = 1.0F );

	//移動量の追加
	//引数...追加の移動量
	void setMove( const float Over );
};

