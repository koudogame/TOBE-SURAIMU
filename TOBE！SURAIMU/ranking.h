#pragma once

// 板場 温樹

#include "scene_base.h"

#include "ranking_manager.h"

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
    void sort( const int Mode );


    bool created_ = false;
    ID3D11ShaderResourceView* texture_numbers_ = nullptr;
    ID3D11ShaderResourceView* texture_characters_ = nullptr;

    std::list<RankingManager::Data> data_;

    float magnification_ = 1.0F;
    float offset_ = 0.0F;
    int sort_mode_ = 0;

	//サウンド
	bool sound_flag_;
	std::unique_ptr<SoundEffectInstance> bgm_;
	std::unique_ptr<SoundEffectInstance> select_se_;
	std::unique_ptr<SoundEffectInstance> dicision_se_;
};
