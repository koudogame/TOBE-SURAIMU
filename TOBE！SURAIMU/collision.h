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
	//判定(プレイヤー対星)
	void collision( Player* , Star* );
	//判定( プレイヤー対壁 )
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
	NameSpaceParticle::ParticleID id_;		//パーティクルID
	bool onece_flag_;						//
	bool start_flag_;						//ゲーム開始かどうかのフラグ
};

