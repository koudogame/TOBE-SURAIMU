#pragma once

// 板場 温樹

#include "scene_base.h"

struct Milliseconds;
template <typename T>
class Timer;

class Pause;
class RankingInEndless;
class Progress;

class StarContainer;
class Player;
class Wall;
class FailWall;

//-----------------------------------------------------------------------------
// エンドレスモード
//-----------------------------------------------------------------------------
class Endless :
    public SceneBase
{
public:
    Endless();
    ~Endless();

    bool init() override;
    void destroy() override;
    SceneBase* update() override;
    void draw() override;

private:
// 関数と関数用の変数
    bool created_ = false;

    SceneBase* start();
    SceneBase* play();
    SceneBase* pause();
    SceneBase* (Endless::* update_)() = nullptr;

    void scroll();
    bool checkAndLoadStage();
    void changePattern( const int PatternNo );


// オブジェクト
    Timer<Milliseconds>* clock_     = nullptr;
    Pause* pause_                   = nullptr;
    RankingInEndless* ranking_      = nullptr;
    StarContainer* star_container_  = nullptr;
    Player* player_                 = nullptr;
    Wall* wall_                     = nullptr;
    FailWall* fail_wall_            = nullptr;
    Progress* progress_             = nullptr;

    int stage_ = 0;
    int round_counter_   = 0;
    bool is_pause_ = false;
    float scroll_threshold_ = 0.0F;
    float offset_ = 0.0F;;
    float offset_one_frame_ = 0.0F;
    float climb_ = 0.0F;
    float player_last_position_y_ = 0.0F;
    float player_displacement_y_sum_ = 0.0F;

	ID3D11ShaderResourceView* description_;
};
