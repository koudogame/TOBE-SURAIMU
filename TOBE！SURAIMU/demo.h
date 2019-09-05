
// î¬èÍÅ@â∑é˜

#pragma once

#include "scene_base.h"

class AIBase;
class Wall;
class FailWall;
class StarContainer;
class Progress;

class Demo :
    public SceneBase
{
public:
    Demo();
    ~Demo();

    static void end() { if( !is_end_ )is_end_ = true; }

    bool init();
    void destroy();
    SceneBase* update();
    void draw();


private:
    static bool is_end_;

    SceneBase* start();
    SceneBase* play();
    SceneBase* (Demo::*update_)() = nullptr;

    void trance(); 

    void scroll();

    bool isCreateStar();
    void setStarPattern();
    bool createStar();

    AIBase* ai_           = nullptr;
    Wall*   wall_         = nullptr;
    FailWall* fail_wall_  = nullptr;
    StarContainer* stars_ = nullptr;
    Progress* progress_   = nullptr;

    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
    bool is_fadein_       = false;
    float alpha_          = 0.0F;
};
