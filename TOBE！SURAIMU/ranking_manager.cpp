
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
    // ランキング外
    if( Score < ranking_[kRegisteredNum - 1U].score ) { return; }
    // ランクイン
    else
    {
        unsigned idx = 0U;

        // 引数のスコア以下の位置まで進める
        for( ; idx < kRegisteredNum; ++idx )
        {
            if( Score >= ranking_[idx].score ) { break; }
        }

        // 登録する
        ranking_.pop_back(); // 最下位をはじき出す
        ranking_.insert( ranking_.begin() + idx,
                         Data{ idx, Player, Score, Height, Combo } );

        // プレイヤー以下の順位を下げる
        for( ; idx < kRegisteredNum; ++idx )
        {
            ++ranking_[idx].rank;
        }
    }
}
