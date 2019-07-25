#include "freeFallParticle_container.h"

const int kTextureSize = 128;		//テクスチャサイズ

FreeFallParticleContainer::FreeFallParticleContainer()
{}


FreeFallParticleContainer::~FreeFallParticleContainer()
{}

//パーティクルの追加
FreeFallParticle * FreeFallParticleContainer::addParticle( const Vector2 & Position, NameSpaceParticle::ParticleID ID, float LifeTime, bool RotateFlag, float Angle )
{
	//空コンテナを取得
	FreeFallParticle* s_particle = getFreeObjAndInsert();
	if( s_particle == nullptr )
		return nullptr;

	RECT trim = { 0,0,kTextureSize,kTextureSize };
	trim.left += kTextureSize * ID;
	trim.right = trim.left + kTextureSize;

	//初期化
	s_particle->init( Position, trim, LifeTime, RotateFlag, Angle );
	return s_particle;
}

//移動量の追加
void FreeFallParticleContainer::setMove( const float Over )
{
	//全アクティブリストに移動量を追加する
	std::list<FreeFallParticle*> list = this->active();

	for( auto& itr : list )
		itr->setMove( Over );
}
