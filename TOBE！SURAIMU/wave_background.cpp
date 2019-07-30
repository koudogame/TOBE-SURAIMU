
#include "wave_background.h"

#include "textureLoder.h"
#include "sprite.h"


/*
    2つのレイヤーからなる。
    メインレイヤーの切り取り範囲は上、補助レイヤーはメインレイヤー切り取り範囲の下にある
    色の変化は、切り取り範囲を左右に移動する
*/

constexpr long kWidth  = 4096L;
constexpr long kHeight = 1200L;
template <typename T>
constexpr T getWidth() { return static_cast<T>( kWidth ); }
template <typename T>
constexpr T getHeight() { return static_cast<T>( kHeight ); }


constexpr Vector2 kOffset { 0.2F, 0.2F };

/*===========================================================================*/
Wave::Wave()
{

}

Wave::~Wave()
{

}


/*===========================================================================*/
bool Wave::init( const Vector2& Position, const Color Color )
{
    // テクスチャ読み込み( 初回のみ )
    if( texture_ == nullptr )
    {
       texture_ = TextureLoder::getInstance()->load(L"Texture/background.png");
       if( texture_ == nullptr ) { return false; }
    }

    // その他メンバの初期化
    position_ = Position;
    offset_ = kOffset;
    if( rand() % 2 ) { offset_.x *= -1.0F; }    // 2分の1の確率で左方向にする


    return true;
}

void Wave::destroy()
{
    // テクスチャ開放
    if( texture_ != nullptr )
    {
        TextureLoder::getInstance()->release( texture_ );
    }
}

void Wave::update()
{
    position_ += offset_;
}

void Wave::draw()
{
}
