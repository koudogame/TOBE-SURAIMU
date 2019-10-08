
// î¬èÍ

#pragma once

#include "scene_base.h"


class StageData;
class Player;
class StarContainer;
class Wall;
class FailWall;

class StagePlay :
    public SceneBase
{
public:
    StagePlay( const std::wstring& StageDataFileName );
    ~StagePlay();

    bool init() override;
    void destroy() override;
    SceneBase* update() override;
    void draw() override;


private:
    std::wstring  stage_data_name_;
    StarContainer *stars_ = nullptr;
    StageData     *data_ = nullptr;
    Player        *player_ = nullptr;
    Wall          *wall_ = nullptr;
    FailWall      *fire_ = nullptr;
};
