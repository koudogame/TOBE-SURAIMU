#pragma once
//*******************************************************************
//Creator：山田　晴久
#include "object_base.h"

//class
class FreeFallParticle :
	public ObjectBase
{
public:
	FreeFallParticle();
	~FreeFallParticle();

	//継承関数
public:
	//初期化
	//引数...描画位置:切り取り範囲
	bool init(const Vector2& Posit ,const RECT& Triming);
	//破棄
	void destroy() override;
	//更新
	void update() override;
	//描画
	void draw() override;
	//生存確認
	bool isAlive() override;

private:
    ID3D11ShaderResourceView* texture_; // テクスチャ
    Vector2 position_;  //座標
	RECT triming_;		//切り取り範囲
	float alpha_;	    //アルファ値
	float angle_;	    //角度
	float move_amount_;	//移動量
	int rotate_;		//回転角
	bool turn_;		    //回転方向
	float fall_;
};

