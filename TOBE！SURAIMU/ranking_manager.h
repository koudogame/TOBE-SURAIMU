#pragma once

// 板場 温樹

constexpr unsigned kRegisteredNum = 100U;


//-----------------------------------------------------------------------------
// ランキング情報管理クラス
//-----------------------------------------------------------------------------
// --説明--
// getInstance関数でインスタンスを取得する
// getScore関数で引数で受け取った順位のスコアを取得する(1U ~ kRegisteredNum)
// setScore関数でランキングにスコアを登録する( ランキング外は登録されない )
class RankingManager
{
private:
    RankingManager();
public:
    ~RankingManager();
    
    struct Data { // ランキングデータ
    unsigned rank;
    std::string name;
    unsigned long long score;
    };


    inline static RankingManager* getInstance()
        { static RankingManager instance; return &instance;}

    void registerScore(const std::string& PlayerName,
                       const unsigned long long& Score);
    inline Data getData(const unsigned Rank) 
        { assert((Rank - 1U) <= 99U && (Rank >= 1U)); return ranking_[Rank - 1]; }

private:
    std::vector<Data> ranking_;
};