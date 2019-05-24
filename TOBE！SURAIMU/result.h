#pragma once

#include "scene_base.h"

class Scoring;

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
	Result(const unsigned Rank, const Scoring& Score);
	~Result();

	bool init() override;
	void destroy() override;
	SceneBase* update() override;
	void draw() override;


private:
    char name_[kNameMax + 1U];
    unsigned index_name_ = 0U;
    unsigned index_char_ = 10U;
    unsigned count_frame_ = 0U;
    SceneBase* setName();
    SceneBase* selectNext();
    SceneBase* (Result::*update_)() = &Result::setName;


    bool created_ = false;
    Vector2 position_;

    const Scoring& score_;
    unsigned rank_ = 101U;
};
