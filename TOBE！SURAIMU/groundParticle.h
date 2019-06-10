#pragma once
//*******************************************************************
//Creator：山田　晴久

#include "object_base.h"
class GroundParticle :
	public ObjectBase
{
public:
	GroundParticle();
	~GroundParticle();

	//継承関数
public:
	//初期化
	//引数...描画位置:切り取り範囲:移動する角度( rad ):拡縮量
	bool init( const Vector2& Posit , const RECT& Triming , const float Angle , const float Scale );
	//破棄
	void destroy() override;
	//更新
	void update()override;
	//描画
	void draw() override;
	//生存確認
	bool isAlive() override;

private:
    ID3D11ShaderResourceView* texture_; //テクスチャ
    Vector2 position_;      //座標
	Vector2 velocity_;		//速度
	RECT triming_;			//切り取り範囲
	float angle_;			//移動角度
	float now_time_;		//入力量
	float scale_;			//拡縮量
};

