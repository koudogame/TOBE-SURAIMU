#pragma once

// 板場 温樹

constexpr int kCharNum = 37;
constexpr char kCharTable[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', ' ',
};

//-----------------------------------------------------------------------------
// テキストクラス
//-----------------------------------------------------------------------------
// --説明--
// 描画機能付き文字クラス
// draw関数で文字の描画を行う
// Texture      : テクスチャ( 横一列 数字->アルファベットの順番で並んでいる )
// DrawPosition : 描画左上位置
// TextWidth    : 1文字の横幅
// TextHeight   : 1文字分の縦幅
// MinCount     : 最低文字数( 左寄り空白詰め )
class Text
{
public:
    static void drawString(
        const std::string& Text,
        ID3D11ShaderResourceView* const Texture,
        const Vector2& DrawPosition,
        const long TextWidth, 
        const long TextHeight,
        const float Alpha = 1.0F);

    static void drawNumber(
        const unsigned long long Number,
        ID3D11ShaderResourceView* const Texture,
        const Vector2& DrawPositionMostRight,
        const long TextWidth,
        const long TextHeight,
        const unsigned MinDigit = 1U,
        const float Alpha = 1.0F,
        const float Interval = 0.0F);

    static int getCharNum(const char);
};