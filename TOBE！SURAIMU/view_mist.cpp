
// 板場

#include "view_mist.h"

#include "textureLoder.h"

// 定数
/*===========================================================================*/
static constexpr wchar_t kTextureFileName[] = { L"Texture/background.png" };
static constexpr RECT  kTrimming[] = 
{
    {    0L,  1800L,  900L, 2700L }, // purple
    {  900L,  1800L, 1800L, 2700L }, // blue
    { 1800L,  1800L, 2700L, 2700L }, // red
};
static constexpr float kDrawDepth = 0.0F;
static constexpr float kScrollSpeed = 1.0F;

static constexpr long kWidth  = 900L;
static constexpr long kHeight = 900L;
template <typename T>
constexpr T getWidth()  { return static_cast<T>(ViewMist::kWidth); }
template <typename T>
constexpr T getHeight() { return static_cast<T>(ViewMist::kHeight); }

/*===========================================================================*/
ViewMist::ViewMist()
{

}

ViewMist::~ViewMist()
{

}


// override
/*===========================================================================*/
// 初期化処理
bool ViewMist::init( const Vector2& Position, const Color Color )
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

    return true;
}
// 終了処理
void ViewMist::destroy()
{
    // テクスチャの開放
    if( texture_ != nullptr )
    {
        TextureLoder::getInstance()->release( texture_ );
    }
}
// 更新処理
void ViewMist::update()
{
    ViewBase::scroll( kScrollSpeed );
}
// 描画処理
void ViewMist::draw()
{
    ViewBase::draw( kTrimming[color_], kDrawDepth );   
}
