#pragma once
//*******************************************************************
//CreatorName:山田　晴久

#include "object_base.h"
#include "shape.h"
#include "groundParticle_container.h"
#include "freeFallParticle_container.h"
#include "stay_particle_container.h"
#include "scoring.h"


class Player:
	public ObjectBase
{
public:
	Player();
	~Player();

public:
	//継承関数
	//初期化
	//引数...初期位置:最大移動量:入力量の追加量:重力:左右の移動量
	virtual bool init( const Vector2& Posit, const int PlayerNo );
	//破棄
	virtual void destroy()override;
	//更新
	virtual void update()override;
	//描画
	virtual void draw() override;
	//生存確認
	bool isAlive() override;
	//移動量の追加
	void setMove( const float Over ) override;
	//現在の位置の取得
	const Vector2& getPosition() const override { return myshape_.position; }
	//自身のIDの返却
	inline ObjectID getID() const override { return ObjectID::kPlayer; }

public:
	//外部利用関数
	//形状の返却
	inline Circle* getShape() { return &myshape_; }
	//移動ベクトルの返却
	inline Line* getMove() { return &move_vector_; }
	//地面の設定
	//引数...地面
	inline void setGround( Line* const Ground ) { died_flag_ ? ground_ = &kGround : ground_ = Ground; }
	//判定を取っていいのかのフラグ
	inline bool isCollision() { return flag_.test( Flag::kStarCollision ); }
	inline bool isWallCollision() { return flag_.test( Flag::kWallCollision ); }
	inline void resetWallCollisionFlag() { flag_.reset( Flag::kWallCollision ); }
	//飛んだかのフラグ
	inline bool isJump() { return flag_.test( Flag::kJump ); }
	//現在の定在する星野返却
	inline ObjectBase* getOwner() { return owner_; }
	//座標の補正
	//引数...交点:パーティクルID
	void revision( const Vector2& CrossPoint, NameSpaceParticle::ParticleID ID );
	//判定後の処理( 星 )
	void collision( class Star* );
	//判定後の処理( 壁 )
	void collision( class Wall* );
	//判定後の処理( プレイヤー )
	void collision( class Player* );
	//回転角の取得
	float getRotate();
	//スコアリングの開始
	inline void onStartFlag() { score_.start(); }
	//上昇量追加
	//引数...上昇量
	inline void addScore( const float AddScore ) { score_.addDefaultScore( AddScore ); }
	//レベルの追加
	inline void addLevel() { score_.addLevel(); }
	//スコアリングのクラス返却
	inline Scoring* getScore() { return &score_; }
	//ガイドのアルファ値返却
	inline float guide() { return guide_alpha_; }


protected:
	//定数化変数
	Line kGround;			//地面

protected:
	//処理系
	//メンバ変数
	Line move_vector_;		//移動ベクトル
	ID3D11ShaderResourceView* texture_; //テクスチャ

	//フラグ
	enum Flag
	{
		kJump,
		kStarCollision,
		kWallCollision,
		kParticle,
		kTechnique,
		kWallParticle,
		kOnce,
		kFlag
	};
	std::bitset<kFlag> flag_;		//フラグ
	float now_amount_;				//入力量
	float ditrection_angle_;		//テクスチャの角度
	float particle_alpha_;
	Line* ground_;					//地面
	ObjectBase* owner_;				//現住する星
	Circle myshape_;				//自分の形
	float dis_;						//衝突時の星の交点の比率
	std::unique_ptr<GroundParticleContainer> g_particle_container_;		//衝突時のパーティクルコンテナ
	std::unique_ptr<FreeFallParticleContainer> f_particle_container_;	//落下物のパーティクルコンテナ
	std::unique_ptr<StayParticleContainer> s_particle_container_;		//プレイヤーのパーティクルコンテナ
	int particle_time_;				//パーティクルの生成時間管理
	float prev_jump_moveamount_;	//１フレーム前の移動量を保存
	int bottom_input_;				//下入力時の倍率
	bool died_flag_;				//死亡フラグ
	float collision_combo_pitch_;	//星の移動時のサウンドのピッチ
	ID3D11ShaderResourceView* guide_;	//ガイドのテクスチャ
	float guide_alpha_;					//ガイドのアルファ値

	Scoring score_;	//スコア
	int player_no_;

	float base_angle_;
	float revision_angle_;
	Vector2 movement_;
	Vector2 offset_;

protected:
	//描画系
	enum Direction
	{
		kFlont,
		kSquat,
		kFlay
	};

	Direction direction_id_;	//プレイヤーの状態
	Vector2 rect_left_up_;		//切り取り左上座標
	//方向の算出
	void slectDirection();

	int timer;		//アニメーション時間管理

protected:
	//内部利用関数
	//入力
	virtual void inputjump();
	virtual void inputmove();
	//重力方向の算出
	void setGravityAngle();
	//**注意**
	//プレイヤーの座標が交点の位置で
	//重力方向が更新された後に呼び出すこと
	void addGroundParticle( NameSpaceParticle::ParticleID ID );
	//落下時のパーティクル生成
	void addFreeFallParticle();
	//死亡時のエフェクト生成
	bool diedEffect();
};