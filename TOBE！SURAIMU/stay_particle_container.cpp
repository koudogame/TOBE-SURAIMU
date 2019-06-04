#include "stdafx.h"
#include "stay_particle_container.h"


StayParticleContainer::StayParticleContainer( Vector2* Posit )
{
	position_ = Posit;
}


StayParticleContainer::~StayParticleContainer()
{}

//パーティクルの追加
StayParticle * StayParticleContainer::addParticle( const int TurnDirection )
{
	//空コンテナを取得
	StayParticle* s_particle = getFreeObjAndInsert();
	if( s_particle == nullptr )
		return nullptr;

	//初期化
	s_particle->init( position_ , TurnDirection );
	return s_particle;
}
