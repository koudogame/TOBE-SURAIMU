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
	//singleton
private:
	Collision();
public:
	static Collision* getInstance() { static Collision instance; return &instance; }

public:
//============================
//外部公開関数
	//初期化
	void init();
	//当たり判定
	bool collision(ObjectBase* Obj1, ObjectBase* Obj2);
	//判定(プレイヤー対星)
	bool collision( Player* , Star* );
	inline bool collision(Star* S, Player* P) { collision(P, S); }
	//判定( プレイヤー対壁 )
	bool collision( Player* , Wall* );
	inline bool collision(Wall* W, Player* P) { collision(P, W); }
	//判定( プレイヤー対プレイヤー)
	bool collision( Player* , Player* );

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
	NameSpaceParticle::ParticleID id_;		//パーティクルID
	bool onece_flag_;						//
	bool start_flag_;						//ゲーム開始かどうかのフラグ
};

