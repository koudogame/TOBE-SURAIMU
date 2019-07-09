#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "shape.h"
#include "groundParticle_container.h"

//判定対象
#include "player.h"
#include "star.h"
#include "wall.h"
#include "ai_demo.h"
#include "sercher.h"
#include "fail_wall.h"

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
	inline bool collision(Star* S, Player* P) { return collision(P, S); }
	inline bool collision(AIDemo* P, Star*S){ return collision(dynamic_cast<Player*>(P), S); }
	inline bool collision(Star* S, AIDemo* P) { return collision(dynamic_cast<Player*>(P), S); }
	//判定( プレイヤー対壁 )
	bool collision( Player* , Wall* );
	inline bool collision(Wall* W, Player* P) { return collision(P, W); }
	//判定( プレイヤー対プレイヤー)
	bool collision( Player* , Player* );
	//判定( CPUの判定範囲対星 )
	bool collision(Sercher*, Star*);
	inline bool collision(Star* S, Sercher* SRS) { return collision(SRS, S); }
	//判定( 死亡ライン )
	bool collision(Player* S, FailWall* FW);
	bool collision(Star* S, FailWall* FW);


private:
	//当たり判定
	bool judgment( const Circle* const, const Circle* const );
	bool judgment( const Circle* const, const Line* const );
	bool judgment( const Line* const , const Line* const );

	//交点を求める
	Vector2 crossPoint( const Circle* const, const Line* const );
	Vector2 crossPoint( const Line* const, const Line* const );

	//メンバ変数
private:
	NameSpaceParticle::ParticleID id_;		//パーティクルID
	bool onece_flag_;						//
	bool start_flag_;						//ゲーム開始かどうかのフラグ
};

