#pragma once
//*******************************************************************
//Creator：山田　晴久

//テクスチャの情報を格納する構造体
struct TitleStatus
{
	Vector2 position;
	ID3D11ShaderResourceView* texture;
	RECT trim;
	float alpha = 1.0F;
	float depth = 1.0F;
};

//class
class TitleObject
{
public:
	TitleObject();
	~TitleObject();

	//継承関数
public:
	//初期化
	bool init( TitleStatus* TitleObjectStatus );
	//描画
	void draw();

private:
	TitleStatus* status_;		//情報格納用変数
};

