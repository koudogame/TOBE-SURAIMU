#pragma once

// 板場 温樹

//-----------------------------------------------------------------------------
// テキストクラス
//-----------------------------------------------------------------------------
// --説明--
// 描画機能付き文字クラス
// draw関数で文字の描画を行う
// Texture      : テクスチャ
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
        const long TextHeight);

    static void drawNumber(
        const unsigned long long Number,
        ID3D11ShaderResourceView* const Texture,
        const Vector2& DrawPositionMostRight,
        const long TextWidth,
        const long TextHeight,
        const unsigned MinDigit = 1U);
};