#include "stdafx.h"
#include "groundParticle_container.h"

//コンストラクタ
GroundParticleContainer::GroundParticleContainer()
{}

//デストラクタ
GroundParticleContainer::~GroundParticleContainer()
{}

//パーティクルの追加
GroundParticle * GroundParticleContainer::addParticle( const std::wstring & FileName , const Vector2 Position , const float Angle )
{
	//フリータスクの取得
	GroundParticle* g_particle = getFreeObjAndInsert();
	if( g_particle == nullptr )
		return nullptr;

	//パーティクルの初期化
	g_particle->init( FileName , Position , Angle );

	return g_particle;
}

void GroundParticleContainer::setMove( const float Over )
{
	std::list<GroundParticle*> list = this->active();

	for( auto& itr : list )
		itr->setMove( Over );
}
