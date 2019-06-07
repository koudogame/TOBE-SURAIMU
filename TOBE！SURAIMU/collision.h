#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "shape.h"
#include "groundParticle_container.h"

//判定対象
#include "player.h"
#include "star.h"
#include "wall.h"
#include "player_demo.h"

class Player;
class Star;
class Wall;
class SerchRangeForStar;

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
	inline bool collision(Star* S, Player* P) { return collision(P, S); }
	inline bool collision(DemoPlayer* P, Star*S){ return collision(dynamic_cast<Player*>(P), S); }
	inline bool collision(Star* S, DemoPlayer* P) { return collision(dynamic_cast<Player*>(P), S); }
	//判定( プレイヤー対壁 )
	bool collision( Player* , Wall* );
	inline bool collision(Wall* W, Player* P) { return collision(P, W); }
	//判定( プレイヤー対プレイヤー)
	bool collision( Player* , Player* );
	//判定( CPUの判定範囲対星 )
	bool collision(SerchRangeForStar*, Star*);
	inline bool collision(Star* S, SerchRangeForStar* SRS) { return collision(SRS, S); }

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

