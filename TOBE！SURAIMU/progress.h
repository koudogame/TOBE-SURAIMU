
// î¬èÍ

#pragma once

#include "object_base.h"


class Player;
class FailWall;
class ProgressStage;
class ProgressPlayer;
class ProgressFailWall;

class Progress 
    : public ObjectBase
{
public:
    Progress();
    ~Progress();

    bool init( const float StageHeight,
               Player* const Player,
               FailWall* const FailWall );

// override
/*=================================================================*/
    void destroy() override;
    void update()  override;
    void draw()    override;
    void setMove(const float Distance) override;
    
    
    void changeStage();
private:

    ProgressStage    *stage_     = nullptr;
    ProgressPlayer   *player_    = nullptr;
    ProgressFailWall *fail_wall_ = nullptr;


    bool isScroll() const;
    void scroll();
    float scroll_sin_               = 0.0F;
    float player_scroll_count_      = 0.0F;
    float last_player_scroll_count_ = 0.0F;
    float stage_scroll_count_       = 0.0F;
    float last_stage_scroll_count_  = 0.0F;
    float offset_                   = 0.0F;
};
