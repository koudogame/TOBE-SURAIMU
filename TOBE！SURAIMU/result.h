#pragma once

#include "scene_base.h"
#include "scoring.h"


namespace {
    constexpr unsigned kNameMax = 8U;
}
//-----------------------------------------------------------------------------
// ƒŠƒUƒ‹ƒg
//-----------------------------------------------------------------------------
class Result :
	public SceneBase
{
public:
	Result(const unsigned Rank, const Scoring Score);
	~Result();

	bool init() override;
	void destroy() override;
	SceneBase* update() override;
	void draw() override;


private:
    bool created_ = false;


    SceneBase* in();
    SceneBase* outToTitle();
    SceneBase* outToPlay();
    SceneBase* setName();
    SceneBase* selectNext();
    SceneBase* (Result::*update_)() = &Result::setName;

    void drawRankingElem( Vector2,
                          const unsigned,
                          const char*,
                          const unsigned long long,
                          const double,
                          const unsigned );

// ‘JˆÚ
    float alpha_ = 1.0F;
    float count_ = 0.0F;
// setName
    char name_[kNameMax + 1U] = {0};
    unsigned index_name_ = 0U;
    int index_char_ = 10U;
    unsigned count_frame_ = 0U;

// ‚»‚Ì‘¼
    ID3D11ShaderResourceView* texture_char_ = nullptr;
    ID3D11ShaderResourceView* texture_char_mini_ = nullptr;
    ID3D11ShaderResourceView* texture_numbers_ = nullptr;
    ID3D11ShaderResourceView* texture_numbers_mini_ = nullptr;
    unsigned rank_ = 101U;
    Scoring score_;
    int select_;
    Vector2 position_base_;

};
