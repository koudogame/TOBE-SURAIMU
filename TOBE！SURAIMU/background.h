#pragma once

#include "object_base.h"

class Background :
    public ObjectBase
{
public:
    Background();
    ~Background();

    bool init()    override;
    void destroy() override;
    void update()  override;
    void draw()    override;
    void setMove( const float Offset ) override { offset_y_ += Offset; }


private:
    ID3D11ShaderResourceView *texture_ = nullptr;

    Vector2 *position_      = nullptr;

    float offset_y_ = 0.0F;
};
