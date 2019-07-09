
// ”Âê@‰·÷

#pragma once

#include "object_base.h"

struct Line;

class FailWall :
    public ObjectBase
{
public:
    FailWall();
    ~FailWall();

    bool init() override;
    void destroy() override;
    void update() override;
    void draw() override;

    void setMove( const float Displacement) override;

    ObjectID getID() const override { return ObjectID::kFailWall; }
    Line* getShape() { return shape_; }

private:
    ID3D11ShaderResourceView* texture_ = nullptr;
    Line* shape_ = nullptr;
};
