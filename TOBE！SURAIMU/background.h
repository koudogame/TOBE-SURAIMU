#pragma once

// î¬èÍ â∑é˜

#include "object_base.h"

//-----------------------------------------------------------------------------
// îwåi
//-----------------------------------------------------------------------------
class Background :
    public ObjectBase
{
public:
    Background(TaskManager* const TaskManager);
    virtual ~Background();

    virtual bool init(const RECT& Trimming,
                      const float Scroll = 1.0F,
                      const float Depth = 0.0F);
    virtual void destroy() override;
    virtual void update() override;
    virtual void draw() override;

    void resetStates(const float Magnification) { magnification_ = Magnification; }

    
protected:
    RECT trimming_{0L, 0L, 0L, 0L};
    float magnification_ = 1.0F;
    float scroll_ = 1.0F;
    float depth_ = 0.0F;
};