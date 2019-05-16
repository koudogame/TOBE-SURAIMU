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
    SceneBase*(Endless::* update_)() = nullptr;

    bool createStar();
    std::deque<std::string> pattern_file_;

    void scoring();
    float prev_player_y_;
    float climb_;

    // オブジェクト
    TaskManager* task_manager_ = nullptr;
    StarContainer* star_container_ = nullptr;
    Player* player_ = nullptr;
    Wall* wall_ = nullptr;

    ID3D11ShaderResourceView* texture_numbers_ = nullptr;
};
