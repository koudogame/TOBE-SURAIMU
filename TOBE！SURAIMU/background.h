#pragma once

#include "object_base.h"

class Background :
    public ObjectBase
{
public:
    Background( const Background& ) = delete;
    Background& operator=( const Background& ) = delete;

    static Background* getInstance() { static Background i; return &i; }

    ~Background();

    bool init()    override;
    void destroy() override;
    void update()  override;
    void draw()    override;
    void setMove( const float Offset ) override { offset_y_ += Offset; }
    void changeColor();

private:
    Background();


    ID3D11ShaderResourceView *texture_ = nullptr;

    Vector2 *position_      = nullptr;
    int color_ = 0;
    float offset_y_ = 0.0F;
};
