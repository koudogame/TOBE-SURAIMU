
#include "view_background.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"

/*
    レイヤーは、添え字の若いほうが奥
    レイヤーの変化は、切り取り範囲を上下に動かす
    色の変化は、切り取り範囲を左右に動かす
*/

constexpr long kWidth = 900L;
constexpr long kHeight = 900L;
template <typename T>
constexpr T getWidth(){ return static_cast<T>(kWidth); }
template <typename T>
constexpr T getHeight() { return static_cast<T>(kHeight); }

constexpr Vector2 kInitPosition
{
    getWindowWidth<float>() * 0.5F - getWidth<float>() * 0.5F,
    -getHeight<float>()
};


constexpr int kLayerNum = 3;
constexpr float kScrollSpeed = 2.0F;
constexpr float kScrollMagnification[kLayerNum]
{
    0.1F,
    0.25F,
    0.5F
};
constexpr float kDrawDepth[kLayerNum]
{
    0.4F,
    0.5F,
    0.6F
};

/*                              left    top   right   bottom  */
constexpr RECT kTrimmingStart {    0L,    0L, kWidth, kHeight };


/*===========================================================================*/
View::View()
{

}

View::~View()
{

}


/*===========================================================================*/
bool View::init( const Vector2& Position, const Color Color )
{
    // テクスチャ読み込み( 初回のみ )
    if( texture_ == nullptr )
    {
       texture_ = TextureLoder::getInstance()->load(L"Texture/background.jpg");
       if( texture_ == nullptr ) { return false; }
    }


    // 初期座標セット
    if( position_ == nullptr )
    {
        // 初回のみ座標用バッファを確保
        position_ = new Vector2[kLayerNum];
    }
    for( int i = 0; i < kLayerNum; ++i )
    {
        position_[i] = kInitPosition;
    }

    return true;
}

void View::destroy()
{
    // 座標用バッファ開放
    if( position_ != nullptr )
    {
        safe_delete_array( position_ );
    }

    // テクスチャ開放
    if( texture_ != nullptr )
    {
        TextureLoder::getInstance()->release( texture_ );
        texture_ = nullptr;
    }
}

void View::update()
{
    // スクロール
    for( int layer = 0; layer < kLayerNum; ++layer )
    {
        position_[layer].y += offset_y_ * kScrollMagnification[layer];

        // 念のため無限に移動し続けないよう処理
        if( position_[layer].y > getWindowHeight<float>() )
        {
            position_[layer].y = getWindowHeight<float>();
        }
    }
}

void View::draw()
{
    Sprite* sprite = Sprite::getInstance();
    RECT trimming = kTrimmingStart;

    for( int layer = 0; layer < kLayerNum; ++layer )
    {
        // レイヤー毎の切り取り範囲の移動処理
        trimming.left += kHeight;
        trimming.bottom += kHeight;

        sprite->reserveDraw(
            texture_,
            position_[layer],
            trimming,
            1.0F,
            kDrawDepth[layer]
        );
    }
}
