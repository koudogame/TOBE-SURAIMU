#pragma once
//*******************************************************************
//Creator：山田　晴久
#include "object_container_base.h"
#include "stay_particle.h"

class StayParticleContainer :
	public ObjectContainerBase<StayParticle>
{
public:
	//コンストラクタ
	//引数:描画位置
	StayParticleContainer(Vector2* Posit );
	~StayParticleContainer();

public:
	//パーティクルの追加
	//回転方向
	StayParticle* addParticle( const int TurnDirection );

private:
	Vector2* position_;		//描画位置
};

