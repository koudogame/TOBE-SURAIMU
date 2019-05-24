#pragma once

// 板場 温樹

#include "object_base.h"

#include "ranking.h"

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

    void setScore(unsigned long long Score) { player_.score = Score; }
    void setMove(const float) override {}

private:
    std::vector<Ranking::Data> ranking_;
    Ranking::Data player_;
};
