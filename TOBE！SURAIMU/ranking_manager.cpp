
// 板場 温樹

#include "ranking_manager.h"

constexpr unsigned kNameMax = 10U;
constexpr unsigned kRangeOfStart = (kRegisteredNum + 1U) / 2U;

/*===========================================================================*/
RankingManager::RankingManager() :
    ranking_(kRegisteredNum)
{
    // ファイルからランキング情報を取得する
    FILE* file = nullptr;
    errno_t error = fopen_s(&file, "State/ranking.txt", "r");
    if( error == 0 && file != nullptr)
    {
        char str[kNameMax];
        // 登録されている情報を格納する
        for( int i = 0; i < kRegisteredNum; ++i)
        {
            // 万が一途中でファイルの終端に達したら取得を終了する
            ranking_[i].rank = i + 1;
            if( fscanf_s(file, 
                "%s %llu %lf %u",
                str, kNameMax,
                &ranking_[i].score,
                &ranking_[i].height,
                &ranking_[i].combo) == EOF)
            {
                break;
            }
            ranking_[i].name.assign(str);
        }

        fclose(file);
    }
}

RankingManager::~RankingManager()
{
    // ファイルにランキング情報を出力する
    FILE* file = nullptr;
    errno_t error = fopen_s(&file, "State/ranking.txt", "w");
    if( error == 0 && file != nullptr)
    {
        // ランキング情報を1位から順に出力している
        for( int i = 0; i < kRegisteredNum; ++i)
        {
            fprintf_s( file,
                "%s %llu %lf %u \n",
                ranking_[i].name.c_str(),
                ranking_[i].score,
                ranking_[i].height,
                ranking_[i].combo);
        }
        
        fclose(file);
    }
}

/*===========================================================================*/
// スコアのセット
void RankingManager::registerScore(
    const std::string& Player,
    const unsigned long long& Score,
    const double Height,
    const unsigned Combo)
{
    unsigned idx = 0U;
    // ランキング外
    if( Score < ranking_[kRegisteredNum - 1].score ) { return; }
    // 1位
    if( Score >= ranking_[0].score ) { idx = 0U; }
    // 最下位
    else if( Score < ranking_[kRegisteredNum - 2U].score ) { idx = kRegisteredNum - 1U; }
    // ランクイン
    else
    {
        // 挿入順位を検索する( 2分探索 )
        unsigned range_size = kRangeOfStart;
        idx = kRangeOfStart;
        while( range_size > 1U )
        {
            range_size  = (range_size + 1U) / 2U;
            if( Score >= ranking_[idx].score )
            {
                if( range_size > idx ) { idx = 0U; }
                else                   { idx -= range_size; }
            }
            else
            {
                idx += range_size;
                if( idx >= kRegisteredNum) { idx = kRegisteredNum; }
            }
        }
    }

    // ランキングに登録する
    ranking_.insert( ranking_.begin() + idx, Data{idx + 1U, Player, Score, Height, Combo} );
    for (++idx; idx < kRegisteredNum; ++idx)
    {
        ++ranking_[idx].rank;
    }
    ranking_.pop_back(); // 最下位は弾き出す
}
