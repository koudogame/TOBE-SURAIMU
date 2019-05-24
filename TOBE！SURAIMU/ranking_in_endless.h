#pragma once

// 板場 温樹

#include "object_base.h"

#include "ranking_manager.h"

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
    ID3D11ShaderResourceView* texture_bar_ = nullptr;
    ID3D11ShaderResourceView* texture_number_ = nullptr;
    ID3D11ShaderResourceView* texture_text_ = nullptr;

    std::vector<RankingManager::Data> ranking_;
    RankingManager::Data player_;

    float displacement_ = 0.0F;
    float disp_frame_ = 0.0F;
};
