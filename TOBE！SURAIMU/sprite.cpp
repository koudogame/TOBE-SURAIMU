
#include "direct3d.h"
#include "commonstates.h"
#include "sprite.h"

//  初期化
bool Sprite::init()
{
    //  SpriteBatchクラスのメモリを確保
	sprite_ = std::make_unique<SpriteBatch>(Direct3D::getInstance()->getContext());
    //  メモリ確保チェック
    if( !sprite_ )
        //  エラー
        return false;
    return true;
}

void Sprite::begin(ID3D11BlendState* BlendState)
{
	sprite_->Begin( SpriteSortMode_BackToFront , BlendState , Common::getInstance()->getStates()->LinearWrap() );
}

void Sprite::draw( ID3D11ShaderResourceView* Texture, const Vector2& Position, const RECT* Rect,float alpha, float Depth, const Vector2& Scale, float Rotate, const Vector2& Anker, SpriteEffects Effect)
{
    sprite_->Draw(
        Texture,
        Position,
        Rect,
        Color(1.0F,1.0F,1.0F,alpha),
        Rotate * XM_PI / 180.0F,
		Anker,
		Scale,
        Effect,
        Depth);
}

void Sprite::end()
{
    sprite_->End();
}