#pragma once
#include "commonstates.h"

class Sprite
{
private:
	Sprite() = default;
    //  Spriteの作成
    unique_ptr<SpriteBatch> sprite_;

public:
	static Sprite* getInstance() { static Sprite instance; return &instance; }
    bool init();     //  初期化
	ID3D11BlendState* chengeMode() { return Common::getInstance()->getStates()->Additive(); }	//描画モードを加算方式に変更
    void begin(ID3D11BlendState* BlendState = Common::getInstance()->getStates()->NonPremultiplied());
    void end();
    //  描画
	void draw(
		ID3D11ShaderResourceView* Texture,				//  テクスチャ
		const Vector2& positioin,                       //  描画位置
		const RECT* Rect = NULL,                        //  切り取り範囲
		float alpha = 1.0F,							    //　α値
		float Depth = 0.0F,                             //  深度値
		const Vector2& Scale = Vector2(1.0F, 1.0F) ,    //	拡縮率
        float Rotate = 0.0F,                            //  回転率
		const Vector2& Anker = Vector2::Zero,		    //	アンカーポイント
        SpriteEffects Effect = SpriteEffects_None );    //  エフェクト

    // SpriteBatchポインタを返す
	SpriteBatch* getSprite() { return sprite_.get(); }
};