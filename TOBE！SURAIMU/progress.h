
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
    void changeStage();


private:
    void easingOffset();

    ID3D11ShaderResourceView *texture_ = nullptr;
    Vector2 stage_position_;
    float stage_height_ = 0.0F;
    float scale_ = 0.0F;
    float offset_ = 0.0F;
    float offset_sin_ = 0.0F;
};
