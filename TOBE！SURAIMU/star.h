#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "object_base.h"
#include "shape.h"
#include "freeFallParticle_container.h"

const int kStarLineNum = 5;

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
	void destroy() override;
	void update() override;
	void draw() override;
	bool isAlive() override;

public:
	//外部利用関数
	//落下の速さの適用
	void setFall();
	inline Line* getShape( const int GetNum ) { return &myshape_[ GetNum ]; }
	void collision( class Player* P );
	void addFreeFallParticle();
	inline void resetStatus( const float Magnification ) { magnification_ = Magnification; }
	inline float getSize() { return size_; }

private:
	//内部利用関数
	void setAngle();

private:
	//メンバ変数
	float angle_[ kStarLineNum ];		//角度
	float fall_;		    //落下の速さ
	float spin_;		    //回転速度
	int turn_;		        //回転方向
	float size_;	        //大きさ
	float rate_;	        //回転倍率
	Line myshape_[ kStarLineNum ];
	int color_val_;			//星のカラーバリエーション

	std::unique_ptr<FreeFallParticleContainer> s_particle_container_;
	int particle_time_;
	int create_point_;
	float magnification_;
};

