#pragma once

// ”Âê ‰·÷

#include "scene_base.h"


class Player;
class Wall;
class StarContainer;

class Battle :
    public SceneBase
{
public:
    Battle();
    ~Battle();

    bool init();
    void destroy();
    SceneBase* update();
    void draw();


private:
    bool created_ = false;

    std::list<Player*> player_;
    Wall* wall_                    = nullptr;
    StarContainer* star_container_ = nullptr;

};