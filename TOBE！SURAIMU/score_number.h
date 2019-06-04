#pragma once
//*******************************************************************
//Creator：山田　晴久

class ScoreNumber
{
public:
	ScoreNumber();
	~ScoreNumber();

public:
	//初期化
	//引数...描画する数値:数値のテクスチャハンドル
	bool init( unsigned int Num , ID3D11ShaderResourceView* Handle );
	//更新
	//引数...描画するY座標
	void update( const float DrawPositY );
	//描画
	void draw();
	//生存確認
	bool isAlive();


private:
	unsigned int number_;			    //数値
	float alpha_;					    //アルファ値
	ID3D11ShaderResourceView* texture_;	//数値のテクスチャ
	ID3D11ShaderResourceView* puluse_;	//プラスのテクスチャ
	Vector2 position_;				    //描画位置
};

