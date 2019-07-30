#pragma once

#include "object_base.h"
#include "back_object_base.h"


class View;
class Wave;
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
    void setMove( const float Offset ) override;
    void changeColor();
    void reset();

private:
    Background();

    std::list<View*> view_list_;
    std::list<View*> view_free_list_;
    std::list<Wave*> wave_list_;
    std::list<Wave*> wave_free_list_;
    View* last_view_ = nullptr;

    BackObjectBase::Color color_ = BackObjectBase::Color::kPurple;
};
