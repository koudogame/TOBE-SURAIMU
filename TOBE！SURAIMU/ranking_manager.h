#pragma once

// 板場 温樹

constexpr unsigned kRegisteredNum = 100U;

//-----------------------------------------------------------------------------
// ランキング情報管理クラス
//-----------------------------------------------------------------------------
// --説明--
// getInstance関数でインスタンスを取得する
// getRanking関数でランキングのコンテナを取得する( 書き換え不可 )
// getScore関数で引数で受け取った順位のスコアを取得する(1U ~ kRegisteredNum)
// registerScore関数でランキングにスコアを登録する( ランキング外は登録されない )
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
        double height;
        unsigned combo;
    };


    inline static RankingManager* getInstance()
       { static RankingManager instance; return &instance;}

    inline const std::vector<Data>& getRanking() { return ranking_; }

    inline Data getData(const unsigned Rank) 
        { assert((Rank - 1U) <= 99U && (Rank >= 1U)); return ranking_[Rank - 1]; }

    void registerScore(const std::string& PlayerName,
                       const unsigned long long& Score,
                       const double Height,
                       const unsigned Combo);

private:
    std::vector<Data> ranking_;
};