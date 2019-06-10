#pragma once
//*******************************************************************
//Creator：山田　晴久
#include "object_base.h"

class StayParticle :
	public ObjectBase
{
public:
	StayParticle();
	~StayParticle();

public:
	//初期化
	//引数...描画座標:回転方向
	bool init( Vector2* Posit , const int TurnDirection );
	//破棄
	void destroy() override;
	//更新
	void update() override;
	//描画
	void draw() override;

private:
    ID3D11ShaderResourceView* texture_; //テクスチャ
	int	turn_;		    //回転方向
	float angle_;	    //角度
	Vector2* position_;	//座標
};

