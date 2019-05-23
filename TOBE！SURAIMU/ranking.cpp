
// 板場 温樹

#include "ranking.h"

constexpr unsigned kRegisteredNum = 100U;
constexpr unsigned kRangeOfStart = (kRegisteredNum + 1U) / 2U;

/*===========================================================================*/
Ranking::Ranking() :
    ranking_(kRegisteredNum, 10U)
{
    // ファイルからランキング情報を取得する
    FILE* file = nullptr;
    errno_t error = fopen_s(&file, "State/ranking.txt", "r");
    if( error == 0 && file != nullptr)
    {
        // 登録されている情報を格納する
        for( int i = 0; i < kRegisteredNum; ++i)
        {
            // 万が一途中でファイルの終端に達したら取得を終了する
            if( fscanf_s(file, "%llu", &ranking_[i]) == EOF)
            {
                break;
            }
        }

        fclose(file);
    }
}

Ranking::~Ranking()
{
    // ファイルにランキング情報を出力する
    FILE* file = nullptr;
    errno_t error = fopen_s(&file, "State/ranking.txt", "w");
    if( error == 0 && file != nullptr)
    {
        // ランキング情報を1位から順に出力している
        for( int i = 0; i < kRegisteredNum; ++i)
        {
            fprintf_s( file, "%llu \n", ranking_[i]);
        }

        fclose(file);
    }
}

/*===========================================================================*/
// スコアのセット
void Ranking::setScore(const ULONGLONG Score)
{
    const auto kBegin = ranking_.begin();


    unsigned idx = 0U;
    // ランキング外
    if( Score < *(kBegin + kRegisteredNum - 1) ) { return; }
    // 1位
    if( Score >= *(kBegin) ) { idx = 0U; }
    // 最下位
    else if( Score < *(kBegin + kRegisteredNum - 2U) ) { idx = kRegisteredNum - 1U; }
    // ランクイン
    else
    {
        // 挿入順位を検索する( 2分探索 )
        unsigned range_size = kRangeOfStart;
        idx = kRangeOfStart;
        while( range_size > 1U )
        {
            range_size  = (range_size + 1U) / 2U;
            if( Score >= *(kBegin + idx) )
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
    ranking_.insert(kBegin + idx, Score);
    ranking_.pop_back(); // 最下位は弾き出す
}
