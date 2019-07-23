#pragma once
#include "direct3d.h"

struct DrawInf
{
	ID3D11ShaderResourceView* texture;
	Vector2 position;
	const RECT* trim;
	float alpha;
	float depth;
	Vector2 scale;
	float rotate;
	Vector2 anker;
	SpriteEffects effect;
	ID3D11BlendState* blend;

	DrawInf(
		ID3D11ShaderResourceView* Texture,
		const Vector2& Position,
		const RECT* Trim,
		float Alpha,
		float Depth,
		const Vector2& Scale,
		float Rotate,
		const Vector2& Anker,
		SpriteEffects Effect,
		ID3D11BlendState* Blend)
	{
		texture = Texture;
		position = Position;
		trim = Trim;
		alpha = Alpha;
		depth = Depth;
		scale = Scale;
		rotate = Rotate;
		anker = Anker;
		effect = Effect;
		blend = Blend;
	}
};

class Sprite
{
private:
	Sprite() = default;
	//  Spriteの作成
	std::unique_ptr<SpriteBatch> sprite_;

public:
	static Sprite* getInstance() { static Sprite instance; return &instance; }
	bool init();     //  初期化
	ID3D11BlendState* chengeMode() { return Common::getInstance()->getStates()->Additive(); }	//描画モードを加算方式に変更
	//  描画
	void draw();

	void reserveDraw(
		ID3D11ShaderResourceView* Texture,				//  テクスチャ
		const Vector2& position,                       //  描画位置
		const RECT* Rect = NULL,                        //  切り取り範囲
		float alpha = 1.0F,							    //　α値
		float Depth = 0.0F,                             //  深度値
		const Vector2& Scale = Vector2(1.0F, 1.0F),    //	拡縮率
		float Rotate = 0.0F,                            //  回転率
		const Vector2& Anker = Vector2::Zero,		    //	アンカーポイント
		ID3D11BlendState* BlendState = Common::getInstance()->getStates()->NonPremultiplied(),	//ブレンドモード
		SpriteEffects Effect = SpriteEffects_None		//エフェクト
	);

	// SpriteBatchポインタを返す
	SpriteBatch* getSprite() { return sprite_.get(); }

private:
	std::vector<DrawInf> draw_inf_;
};