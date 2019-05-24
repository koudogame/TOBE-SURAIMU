#pragma once

// 板場 温樹

#include "object_base.h"

#include "text.h"
#include "numbers.h"

//-----------------------------------------------------------------------------
// エンドレスモード内ランキング管理クラス
//-----------------------------------------------------------------------------
// --説明--
// setScore関数で現在のスコアをセットする
// setMove関数は無効化
class RankingInEndless :
    public ObjectBase
{
public:
    RankingInEndless();
    ~RankingInEndless();

    bool init() override;
    void destroy() override;
    void update() override;
    void draw() override;

    void setScore(ULONGLONG Score) { score_ = Score; }
    void setMove(const float) override {}

private:
    std::vector<std::pair<Text, Numbers<ULONGLONG>>> ranking_;
    Text player_name_;
    Numbers<ULONGLONG> score_ = 0ULL;
    int rank_ = 0U;
};
