#pragma once
//*******************************************************************
//Creator：山田　晴久

//テクスチャの情報を格納する構造体
struct TitleStatus
{
	Vector2 position;
	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* overlay_texture;
	RECT trim;
	float alpha = 1.0F;
	float depth = 1.0F;
	//描画
	void draw();
};

