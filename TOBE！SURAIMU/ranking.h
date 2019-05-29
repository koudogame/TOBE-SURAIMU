#pragma once

// 板場 温樹

#include "scene_base.h"


//-----------------------------------------------------------------------------
// ランキングシーン
//-----------------------------------------------------------------------------
class Ranking :
    public SceneBase
{
public:
    Ranking();
    ~Ranking();

    bool init() override;
    void destroy() override;
    SceneBase* update() override;
    void draw() override;

private:
    bool created_ = false;
    ID3D11ShaderResourceView* texture_numbers_ = nullptr;
    ID3D11ShaderResourceView* texture_characters_ = nullptr;

    float magnification_ = 1.0F;
    float offset_ = 0.0F;

	bool sound_flag_;

	//サウンド
	std::unique_ptr<SoundEffectInstance> bgm_;
	std::unique_ptr<SoundEffectInstance> select_se_;
	std::unique_ptr<SoundEffectInstance> dicision_se_;
};
