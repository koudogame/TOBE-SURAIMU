
// î¬èÍ

#pragma once

#include "object_base.h"


class Player;
class FailWall;
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
    void setMove( const float Distance ) override;

/*=================================================================*/
    void start();
    void changeStage();

private:
    void easingOffset();
    float getPlayerDelta();

    ID3D11ShaderResourceView *texture_ = nullptr;
    float scale_ = 0.0F;

    Vector2 stage_position_;
    float stage_height_ = 0.0F;
    float offset_ = 0.0F;
    float offset_sin_ = 0.0F;

    Player  *player_ = nullptr;
    Vector2 player_base_position_;
    float   player_displacement_ = 0.0F;
    float   player_last_position_y_ = 0.0F;
    float   wall_last_position_y_ = 0.0F;
    float   last_distance_player_wall_ = 0.0F;
    float   scroll_speed_ = 0.0F;


    FailWall *wall_ = nullptr;
};
