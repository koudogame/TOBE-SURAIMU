
// 板場

#include "view_star_big.h"

#include "textureLoder.h"
#include "sprite.h"
#include "collision.h"

#include "fail_wall.h"

// 定数
/*===========================================================================*/
static constexpr wchar_t kTextureFileName[] = { L"Texture/background.png" };
static constexpr RECT    kTrimming[] = 
{
    {    0L,  900L,  900L, 1800L }, // purple
    {  900L,  900L, 1800L, 1800L }, // blue
    { 1800L,  900L, 2700L, 1800L }, // red
};
static constexpr float kDrawDepth = 0.2F;
static constexpr float kScrollSpeed = 0.5F;


static constexpr long kWidth  = 900L;
static constexpr long kHeight = 900L;
template <typename T>
constexpr T getWidth() { return static_cast<T>(kWidth); }
template <typename T>
constexpr T getHeight() { return static_cast<T>(kHeight); }

/*===========================================================================*/
ViewStarBig::ViewStarBig()
{

}

ViewStarBig::~ViewStarBig()
{

}


// override
/*===========================================================================*/
// 初期化処理
bool ViewStarBig::init( const Vector2& Position, const Color Color )
{
    // テクスチャの読み込み
    if( texture_ == nullptr )
    {
        texture_ = TextureLoder::getInstance()->load( kTextureFileName );
        if( texture_ == nullptr ) { return false; } // エラー
    }

    // その他メンバ初期化
    color_ = Color;
    position_ = Position;
    is_alive_ = true;


    return true;
}
// 終了処理
void ViewStarBig::destroy()
{
    // テクスチャ開放
    if( texture_ != nullptr ) 
    {
        TextureLoder::getInstance()->release( texture_ );
    }
}
// 更新処理
void ViewStarBig::update()
{
    position_.y += kScrollSpeed;


    if (fail_wall_)
    {
        if (Collision::getInstance()->collision(this, fail_wall_))
        {
            is_alive_ = false;
        }
    }
    else
    {
        if (position_.y > getWindowHeight<float>())
        {
            is_alive_ = false;
        }
    }
}
// 描画処理
void ViewStarBig::draw()
{
    if( (position_.y < getWindowHeight<float>()) )
    {
        Sprite::getInstance()->reserveDraw(
            texture_,
            position_,
            kTrimming[color_],
            1.0F, // alpha
            kDrawDepth
        );
    }
}
