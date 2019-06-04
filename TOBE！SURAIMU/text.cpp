
// 板場 温樹

#include "text.h"

#include "sprite.h"

/*===========================================================================*/
// 文字列描画
void Text::drawString( const std::string& Text,
                       ID3D11ShaderResourceView* const Texture,
                       const Vector2& Position,
                       const long Width, const long Height,
                       const float Alpha )
{
    Sprite* kSprite = Sprite::getInstance();

    Vector2 position = Position;
    RECT trimming {0L, 0L, Width, Height};

    for( auto ch : Text )
    {
        trimming.left = getCharNum( ch ) * Width;
        trimming.right = trimming.left + Width;

        kSprite->draw( Texture, position, &trimming, Alpha, 0.9F );

        position.x += Width;
    }
}
// 数列描画
void Text::drawNumber( const unsigned long long Number,
                       ID3D11ShaderResourceView* const Texture,
                       const Vector2& Position,
                       const long Width, const long Height,
                       const unsigned Digit,
                       const float Alpha,
                       const float Interval)
{
    Vector2 position = Position;
    position.x -= Width;

    RECT trimming {0L, 0L, Width, Height};

    unsigned long long temp = Number;

    // 1の桁から描画
	// 指定された桁数より小さいか、数値が0より大きい間描画する
    for( unsigned i = 0U; (i < Digit) || (temp > 0ULL); ++i )
    {
        trimming.left = static_cast<long>(temp % 10ULL) * Width;
        trimming.right = trimming.left + Width;

        Sprite::getInstance()->draw(
            Texture,
            position,
            &trimming,
            Alpha,
            0.9F
        );

        temp /= 10ULL;
        position.x -= Width + Interval;
    }
}

/*===========================================================================*/
// 引数の文字のキャラテーブル上の値を返す
int Text::getCharNum( const char Char )
{
    for( int i = 0; i < kCharNum; ++i )
    {
        if(kCharTable[i] == Char)
        {
            return i;
        }
    }

    return -1;
}
