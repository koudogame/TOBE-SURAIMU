#pragma once
//*******************************************************************
//Creator：山田　晴久
#include "object_container_base.h"
#include "freeFallParticle.h"

class FreeFallParticleContainer :
	public ObjectContainerBase<FreeFallParticle>
{
public:
	FreeFallParticleContainer();
	~FreeFallParticleContainer();

	//継承関数
public:
	//パーティクルの追加
	//引数...描画位置:パーティクルID
	FreeFallParticle* addParticle( const Vector2& Position ,
								   NameSpaceParticle::ParticleID ID,
								   float Alpha = 1.0F);
	//移動量の追加
	//引数...移動量
	void setMove( const float Over );
};

