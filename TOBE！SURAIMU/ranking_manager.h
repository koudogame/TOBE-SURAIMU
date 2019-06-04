#pragma once

// 板場 温樹

constexpr unsigned kRegisteredNum = 100U;

//-----------------------------------------------------------------------------
// ランキング情報管理クラス
//-----------------------------------------------------------------------------
// --説明--
// getInstance関数   : 実体へのインスタンスを取得する
// getRanking関数    : ランキングデータを取得する( 書き換え不可 )
// getData関数       : 指定した順位のデータを取得する
// registerScore関数 : ランキングにデータを登録する( ランキング外は登録されない )
class RankingManager
{
private:
    // Singleton
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
         { assert((Rank - 1U) < kRegisteredNum && (Rank >= 1U)); return ranking_[Rank - 1]; }

    void registerScore(const std::string& PlayerName,
                       const unsigned long long& Score,
                       const double Height,
                       const unsigned Combo);

private:
    std::vector<Data> ranking_;
};