
// 板場 温樹

#include "text.h"

#include "sprite.h"

int getCharNum(const char);

constexpr int kCharNum = 37;
constexpr char kCharTable[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', ' ',
};

/*===========================================================================*/
Text::Text()
{
}
// 文字セットコンストラクタ
Text::Text( const char Char )              { text_ = Char; }
Text::Text( const char* String )          { text_ = String; }
// 数値セットコンストラクタ
Text::Text( const int Num )                { text_ = std::to_string(Num); }
Text::Text( const unsigned Num )           { text_ = std::to_string(Num); }
Text::Text( const unsigned long& Num)      { text_ = std::to_string(Num); }
Text::Text( const unsigned long long& Num) { text_ = std::to_string(Num); }

Text::~Text()
{
}

/*===========================================================================*/
// 描画
void Text::draw( ID3D11ShaderResourceView* Texture,
                 const Vector2& Position,
                 const long Width, const long Height )
{
    Sprite* kSprite = Sprite::getInstance();

    Vector2 position = Position;
    RECT trimming {0L, 0L, Width, Height};


    for( auto ch : text_ )
    {
        trimming.left = getCharNum( ch ) * Width;
        trimming.right = trimming.left + Width;

        kSprite->draw( Texture, position, &trimming, 1.0F, 0.0F );

        position.x += Width;
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