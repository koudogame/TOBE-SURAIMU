#pragma once

// 板場 温樹

#include "scene_base.h"

struct Milliseconds;
template <typename T>
class Timer;

class StarContainer;
class Player;
class Wall;

class RankingInEndless;

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
    bool create();
    bool do_create_ = true;

    SceneBase* start();
    SceneBase* play();
    SceneBase* (Endless::* update_)() = nullptr;

    void adjustObjectPosition(const float);
    bool checkAndCreateStar();


// オブジェクト
    Timer<Milliseconds>* clock_                     = nullptr;
    RankingInEndless* ranking_                      = nullptr;
    StarContainer* star_container_                  = nullptr;
    Player* player_                                 = nullptr;
    Wall* wall_                                     = nullptr;

    float magnification_ = 1.0F;
    float climb_ = 0.0F;
};
