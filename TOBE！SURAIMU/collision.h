#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "shape.h"
#include "groundParticle_container.h"

class Player;
class Star;
class Wall;

class Collision
{
public:
	~Collision();
private:
	//singleton
	Collision();

public:
//============================
//外部公開関数
	static Collision* getInstance() { static Collision instance; return &instance; }
	void collision( Player* , Star* );
	void collision( Player* , Wall* );

private:
	//当たり判定
	bool judgment( Circle* , Circle* );
	bool judgment( Circle* , Line* );
	bool judgment( Line* , Line* );

	//交点を求める
	Vector2 crossPoint( Circle* , Line* );
	Vector2 crossPoint( Line* , Line* );

	//メンバ変数
private:
	NameSpaceParticle::ParticleID id_;
};

