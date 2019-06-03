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
// ランキングデータ構造体
    struct Data {
        unsigned rank;
        std::string name;
        unsigned long long score;
    };
public:
    RankingInEndless();
    ~RankingInEndless();

    bool init() override;
    void destroy() override;
    void update() override;
    void draw() override;

    void drawData(
        const Data& Data,
        Vector2 Position,
        const RECT* const TrimminaBar,
        const float Alpha);

    inline void setScore(unsigned long long Score) { player_.score = Score; }
    inline void setMove(const float) override {}
    inline unsigned getRank() { return player_.rank; }

private:
    bool created_ = false;

    ID3D11ShaderResourceView* texture_bar_              = nullptr;
    ID3D11ShaderResourceView* texture_number_           = nullptr;
    ID3D11ShaderResourceView* texture_number_forplayer_ = nullptr;
    ID3D11ShaderResourceView* texture_text_             = nullptr;


    std::vector<Data> ranking_;
    Data player_;

    float displacement_ = 0.0F;
    float disp_frame_   = 0.0F;
};
