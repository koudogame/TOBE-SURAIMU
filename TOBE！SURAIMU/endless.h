#pragma once

// 板場 温樹

#include "scene_base.h"

struct Milliseconds;
template <typename T>
class Timer;

class ObjectBase;
class BackgroundContainer;
class BackObjectContainer;
class StarContainer;
class Player;
class Wall;

class Combo;

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

    bool createStar();
    std::deque<std::wstring> pattern_file_;

    void adjustObjectPosition(const float);

    void scoring();
    float climb_;
    ObjectBase* prev_player_owner_ = nullptr;
    bool prev_player_jump_state_;

// オブジェクト
    Timer<Milliseconds>* clock_                     = nullptr;
    StarContainer* star_container_                  = nullptr;
    Player* player_                                 = nullptr;
    Wall* wall_                                     = nullptr;
    Combo* combo_                                   = nullptr;

    float magnification_;

    ID3D11ShaderResourceView* texture_numbers_ = nullptr;
};
