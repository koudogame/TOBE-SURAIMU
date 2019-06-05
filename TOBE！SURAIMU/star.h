#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "object_base.h"
#include "shape.h"
#include "freeFallParticle_container.h"

const int kStarLineNum = 5;		//星を形成する線分の数

class Star :
	public ObjectBase
{
public:
	Star();
	~Star();

public:
	//継承関数
	//初期化
	//引数...初期座標 : 初期角度 : 回転の速さ : 回転倍率 : 大きさ
	bool init( const Vector2& Position , const float Angle , const float Spin , const float Rate , const float Size );
	//破棄
	void destroy() override;
	//更新
	void update() override;
	//描画
	void draw() override;
	//生存確認
	bool isAlive() override;

public:
	//外部利用関数
	//落下の速さの適用
	void setFall();
	//形の取得
	//引数...取得線分
	inline Line* getShape( const int GetNum ) { return &myshape_[ GetNum ]; }
	//判定後の処理( プレイヤー )
	void collision( class Player* P );
	//落下パーティクルの生成
	void addFreeFallParticle();
	//サイズの取得
	inline float getSize() { return size_; }
	//パーティクルIDの取得
	inline NameSpaceParticle::ParticleID  getColor() { return id_; }

private:
	//内部利用関数
	//角度の設定
	void setAngle();

private:
	//メンバ変数
	float angle_[ kStarLineNum ];		//角度
	float fall_;		    //落下の速さ
	float spin_;		    //回転速度
	float size_;	        //大きさ
	float rate_;	        //回転倍率
	Line myshape_[ kStarLineNum ];		//自分の形

	std::unique_ptr<FreeFallParticleContainer> s_particle_container_;		//落下パーティクルコンテナ
	int particle_time_;	//パーティクルの生成時間管理
	int create_point_;	//パーティクルの生成位置
	NameSpaceParticle::ParticleID id_;	//パーティクルID

	ID3D11ShaderResourceView* overlay_texture_;	//星の加算合成テクスチャ
};

