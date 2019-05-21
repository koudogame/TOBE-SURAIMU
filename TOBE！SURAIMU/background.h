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

    virtual bool init(const wchar_t* const TextureFileName, const RECT& Trimming, const float Scroll);
    virtual void destroy() override;
    virtual void update() override;
    virtual void draw() override;

    void resetStates(const float Magnification) { magnification_ = Magnification; }

    
protected:
    RECT trimming_;
    float magnification_;
    float scroll_;
};