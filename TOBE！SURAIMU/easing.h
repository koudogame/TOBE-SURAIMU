#pragma once
//========================================
//CreatorName:山田　晴久
class Easing
{
	//singleton
private:
	Easing() = default;
public:
	static Easing* getInstance() { static Easing instance; return &instance; }

public:
	~Easing();

public:
	enum Mode
	{
		In,
		Out,
		InOut	//0.5を境にINとOUTが反転する
	};

	//引数
	//最大移動量：現在の時間( 0.0 ～ 1.0 )：出力モード
	float expo( const float Movement, const float Time , Mode Mode );


};

