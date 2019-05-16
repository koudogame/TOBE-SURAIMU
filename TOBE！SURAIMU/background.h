#pragma once

#include "object_base.h"

//-----------------------------------------------------------------------------
// îwåi
//-----------------------------------------------------------------------------
class Background :
    public ObjectBase
{
public:
    Background(TaskManager* const TaskManager);
    ~Background();

    bool init(const wchar_t* const TextureFileName, const float Scroll);
    void destroy() override;
    void update() override;
    void draw() override;

private:
    float scroll_;
};