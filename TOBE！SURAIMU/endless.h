#pragma once

// 板場 温樹

#include "scene_base.h"

class TaskManager;
class StarContainer;
class Player;
class Wall;

//-----------------------------------------------------------------------------
// エンドレスモード
//-----------------------------------------------------------------------------
class Endless :
    public SceneBase
{
public:
    Endless();
    ~Endless();
// メンバ関数
    bool init() override;
    void destroy() override;
    SceneBase* update() override;
    void draw() override;

private:
// 関数と関数用の変数
    bool create();
    bool do_create_ = true;

    bool createStar();
    std::deque<std::string> pattern_file_;

    SceneBase* play();
    SceneBase*(Endless::* update_)() = nullptr;


// オブジェクト
    TaskManager* task_manager_ = nullptr;
    StarContainer* star_container_ = nullptr;
    Player* player_ = nullptr;
    Wall* wall_ = nullptr;
};
