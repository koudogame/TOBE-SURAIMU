#pragma once

// 板場 温樹

//-----------------------------------------------------------------------------
// ランキング情報管理クラス
//-----------------------------------------------------------------------------
// --説明--
// getInstance関数でインスタンスを取得する
// 
class Ranking
{
private:
    Ranking();
public:
    ~Ranking();

    inline static Ranking* getInstance() 
        { static Ranking instance; return &instance;}

    void setScore(const ULONGLONG Score);
    inline ULONGLONG getScore(const unsigned Rank) 
        { assert((Rank - 1) > 99); return ranking_[Rank - 1]; }

private:
    std::vector<ULONGLONG> ranking_;
};