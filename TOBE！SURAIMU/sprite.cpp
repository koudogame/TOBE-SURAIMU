
#include "direct3d.h"
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
	draw_inf_.clear();
    return true;
}

void Sprite::draw()
{
	std::sort(draw_inf_.begin(), draw_inf_.end(), [](const DrawInf& a, const DrawInf& b)->bool {return a.depth < b.depth; });

	ID3D11BlendState* blend_mode = nullptr;
	for (auto& itr : draw_inf_)
	{
		auto temp = itr.blend;
		if (blend_mode != temp)
		{
			if (blend_mode != nullptr)
				sprite_->End();
			sprite_->Begin(SpriteSortMode_Deferred, temp);
			blend_mode = temp;
		}

		sprite_->Draw(
			itr.texture,
			itr.position,
			itr.trim,
			Color(1.0F, 1.0F, 1.0F, itr.alpha),
			itr.rotate,
			itr.anker,
			itr.scale,
			itr.effect,
			itr.depth);
	}
	sprite_->End();

	draw_inf_.clear();
}

void Sprite::reserveDraw(ID3D11ShaderResourceView * Texture, const Vector2 & Position, const RECT * Rect, float alpha, float Depth, const Vector2 & Scale, float Rotate, const Vector2 & Anker, ID3D11BlendState * BlendState, SpriteEffects Effect)
{
	draw_inf_.push_back(DrawInf(Texture, Position, Rect, alpha, Depth, Scale, Rotate, Anker, Effect,BlendState));
}