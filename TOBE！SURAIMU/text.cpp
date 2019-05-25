
// ”Âê ‰·÷

#include "text.h"

#include "sprite.h"

int getCharNum(const char);

/*===========================================================================*/
// •¶š—ñ•`‰æ
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

        kSprite->draw( Texture, position, &trimming, Alpha, 0.0F );

        position.x += Width;
    }
}
// ”—ñ•`‰æ
void Text::drawNumber( const unsigned long long Number,
                       ID3D11ShaderResourceView* const Texture,
                       const Vector2& Position,
                       const long Width, const long Height,
                       const unsigned Digit,
                       const float Alpha)
{
    Vector2 position = Position;
    position.x -= Width;

    RECT trimming {0L, 0L, Width, Height};

    unsigned long long temp = Number;

    // 1‚ÌŒ…‚©‚ç•`‰æ
	// w’è‚³‚ê‚½Œ…”‚æ‚è¬‚³‚¢‚©A”’l‚ª0‚æ‚è‘å‚«‚¢ŠÔ•`‰æ‚·‚é
    for( unsigned i = 0U; (i < Digit) || (temp > 0ULL); ++i )
    {
        trimming.left = static_cast<long>(temp % 10ULL) * Width;
        trimming.right = trimming.left + Width;

        Sprite::getInstance()->draw(
            Texture,
            position,
            &trimming,
            Alpha,
            1.0F
        );

        temp /= 10ULL;
        position.x -= Width;
    }
}

int getCharNum( const char Char )
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
