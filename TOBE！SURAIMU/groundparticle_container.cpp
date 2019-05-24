#include "stdafx.h"
#include "groundParticle_container.h"

const int kTextureSize = 64;

//コンストラクタ
GroundParticleContainer::GroundParticleContainer()
{}

//デストラクタ
GroundParticleContainer::~GroundParticleContainer()
{}

//パーティクルの追加
GroundParticle * GroundParticleContainer::addParticle(const Vector2 Position , const float Angle, ParticleID ID )
{
	if( ID == ParticleID::kNonParticle )
		return nullptr;

	//フリータスクの取得
	GroundParticle* g_particle = getFreeObjAndInsert();
	if( g_particle == nullptr )
		return nullptr;

	RECT trim = { 0,0,kTextureSize,kTextureSize };
	trim.left += kTextureSize * ID;
	trim.right = trim.left + kTextureSize;

	//パーティクルの初期化
	g_particle->init( Position , trim , Angle );

	return g_particle;
}

void GroundParticleContainer::setMove( const float Over )
{
	std::list<GroundParticle*> list = this->active();

	for( auto& itr : list )
		itr->setMove( Over );
}
