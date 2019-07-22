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

    int (Pause::*update_ ) ();
    int waitInput();
    int toContinue();
    int toRestart();
    int toTitle();

    ID3D11ShaderResourceView* texture_ = nullptr;
    Vector2 position_cursor_;
    int select_ = kKeep;
    bool count_started_ = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> count_start_;
    long remaining_count_ = 0L;
    float number_scale_ = 1.0F;
    float number_alpha_ = 1.0F;
};
