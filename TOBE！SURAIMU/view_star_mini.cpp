
#include "view_star_mini.h"

#include "textureLoder.h"

/*===========================================================================*/
static constexpr wchar_t kTextureFileName[] = { L"Texture/background.png" };
static constexpr RECT    kTrimming[] = 
{
    {    0L,    0L,  900L,  900L }, // purple
    {  900L,    0L, 1800L,  900L }, // blue
    { 1800L,    0L, 2700L,  900L }, // red
};
static constexpr float kDrawDepth = 0.1F;
static constexpr float kScrollSpeed = 0.5F;


static constexpr long kWidth  = 900L;
static constexpr long kHeight = 900L;
template <typename T>
constexpr T getWidth(){ return static_cast<T>(ViewStarMini::kWidth); }
template <typename T>
constexpr T getHeight() { return static_cast<T>(ViewStarMini::kHeight); }

/*===========================================================================*/
ViewStarMini::ViewStarMini()
{

}

ViewStarMini::~ViewStarMini()
{

}


// override
/*===========================================================================*/
// 初期化処理
bool ViewStarMini::init( const Vector2& Position, const Color Color )
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
void ViewStarMini::destroy()
{
    // テクスチャ開放
    if( texture_ != nullptr )
    {
        TextureLoder::getInstance()->release( texture_ );
    }
}
// 更新処理
void ViewStarMini::update()
{
    ViewBase::scroll( kScrollSpeed );
}
// 描画処理
void ViewStarMini::draw()
{
    ViewBase::drawSeamless( kTrimming[color_], kDrawDepth );
}
