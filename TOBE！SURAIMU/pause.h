#pragma once

//-----------------------------------------------------------------------------
// ポーズ
//-----------------------------------------------------------------------------
// --説明--
// update関数戻り値
//  kKeep     : ポーズ継続
//  kContinue : ゲーム続行
//  kRestart  : 最初から
//  kTitle    : タイトルへ
class Pause
{
public:
    enum : int
    {
        kKeep = -1,
        kContinue,
        kRestart,
        kTitle,
    };

    Pause();
    ~Pause();

    bool init();
    void destroy();
    int  update();
    void draw() const;

    void reset();

private:
    bool created_ = false;

    ID3D11ShaderResourceView* texture_ = nullptr;
    ID3D11ShaderResourceView* texture_back_ = nullptr;
    Vector2 position_cursor_;
    int select_ = kContinue;
};