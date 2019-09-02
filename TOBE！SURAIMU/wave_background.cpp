
#include "wave_background.h"

#include "textureLoder.h"
#include "sprite.h"

/*
    2つのレイヤーからなる。
    メインレイヤーの切り取り範囲は上、エフェクトはメインレイヤー切り取り範囲の下にある
    色の変化は、画像を切り替える

    テクスチャの構成は
        メインレイヤー範囲( 4096 * 4096 )
            ・パターン1( 4096 * 1200 )
            ・パターン2( 4096 * 1200 )
            ・パターン3( 4096 * 1200 )
                    ・
                    ・
                    ・
        エフェクト範囲   ( 4096 * 4096 )
            ・パターン1( 4096 * 1200 )
            ・パターン2( 4096 * 1200 )
            ・パターン3( 4096 * 1200 )
                    ・
                    ・
                    ・
*/
const wchar_t* const kTextureFileName[]
{
    L"Texture/back_wave_purple.png",    // kPurple
    L"Texture/back_wave_blue.png",      // kBule
    L"Texture/back_wave_red.png",       // kRed
};

constexpr long kWidth  = 4096L;
constexpr long kHeight = 1200L;
template <typename T>
constexpr T getWidth() { return static_cast<T>( kWidth ); }
template <typename T>
constexpr T getHeight() { return static_cast<T>( kHeight ); }

constexpr int kPatternNum = 3;

constexpr long kTrimDeltaMainToEffect = 4096L;
/*                                        left    top   right   bottom */
constexpr RECT kTrimmingStartMainLayer {    0L,    0L, kWidth, kHeight };
constexpr RECT kTrimmingStartEffect    {    0L, kTrimDeltaMainToEffect, kWidth, kTrimDeltaMainToEffect + kHeight };
constexpr float kAlphaMainLayer = 0.5F;
constexpr float kDrawDepthMainLayer = 0.4F;
constexpr float kDrawDepthEffect    = 0.5F;

constexpr Vector2 kOffset { 0.2F, 0.2F };
constexpr float kAmountOfEffectAlpha = 0.015F;
constexpr float kEffectAlphaMax = 1.0F;
constexpr float kEffectAlphaMin = 0.2F;

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

    destroy();

    // テクスチャ読み込み( 初回のみ )
    texture_ = TextureLoder::getInstance()->load(kTextureFileName[Color]);
    if( texture_ == nullptr ) { return false; }

    // その他メンバの初期化
    effect_ = &Wave::effectFadeOut;
    position_ = Position;
    offset_ = kOffset;
    if( rand() % 2 ) { offset_.x *= -1.0F; }    // 2分の1の確率で左方向にする
    pattern_id_ = rand() % kPatternNum;
    effect_alpha_ = 1.0F;

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

    (this->*effect_)();
}

void Wave::draw()
{
    Sprite* sprite = Sprite::getInstance();

    RECT trimming = kTrimmingStartMainLayer;
    // パターン変更
    trimming.top += pattern_id_ * kHeight;
    trimming.bottom = trimming.top + kHeight;
    // カラー変更
    trimming.left += color_ * getWidth<long>();
    trimming.right = trimming.left + getWidth<long>();


    auto draw = [sprite, trimming, this]( const Vector2& Position )
    {
        // メインレイヤー描画
        sprite->reserveDraw(
            texture_,
            Position,
            trimming,
            kAlphaMainLayer,
            kDrawDepthMainLayer,
            { 1.0F, 1.0F }, 0.0F, Vector2::Zero,
            Common::getInstance()->getStates()->Additive()
        );

        // エフェクト描画
        sprite->reserveDraw(
            texture_,
            Position,
            { 
                trimming.left, 
                trimming.top + kTrimDeltaMainToEffect, 
                trimming.right,
                trimming.bottom + kTrimDeltaMainToEffect
            },
            effect_alpha_,
            kDrawDepthEffect,
            {1.0F, 1.0F}, 0.0F, Vector2::Zero,
            Common::getInstance()->getStates()->Additive()
        );
    };


    // シームレスに描画
    Vector2 draw_position = position_;

    // 左方向に描画
    while( draw_position.x + getWidth<float>() > 0.0F )
    {
        draw( draw_position );

        draw_position.x -= getWidth<float>();
    }
    // 右方向に描画
    draw_position = position_;
    draw_position.x += getWidth<float>();
    while( draw_position.x - getWidth<float>() < getWindowWidth<float>() )
    {
        draw( draw_position );

        draw_position.x += getWidth<float>();
    }
}

/*===========================================================================*/
void Wave::setColor( const Color Color )
{
    if( texture_ )
    {
        auto tex_manager = TextureLoder::getInstance();

        tex_manager->release( texture_ );
        texture_ = tex_manager->load( kTextureFileName[Color] );
    }
}

/*===========================================================================*/
void Wave::effectFadeIn()
{
    effect_alpha_ += kAmountOfEffectAlpha;

    if( effect_alpha_ >= kEffectAlphaMax )
    {
        effect_alpha_ = kEffectAlphaMax;
        effect_ = &Wave::effectFadeOut;
    }
}

void Wave::effectFadeOut()
{
    effect_alpha_ -= kAmountOfEffectAlpha;

    if( effect_alpha_ <= kEffectAlphaMin )
    {
        effect_alpha_ = kEffectAlphaMin;
        effect_ = &Wave::effectFadeIn;
    }
}
