
// î¬èÍ

#pragma once

#include "object_base.h"
#include "back_object_base.h"


class ViewMist;
class ViewStarMini;
class ViewStarBig;
class ViewWave;
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

    template <typename T>
    bool updateView( std::vector<T*> *List, std::vector<T*> *FreeList );

    bool isCreateWave() const;
    bool updateWave();


    std::vector<ViewMist*>     mist_list_;
    std::vector<ViewMist*>     mist_free_;
    std::vector<ViewStarMini*> mini_star_list_;
    std::vector<ViewStarMini*> mini_star_free_;
    std::vector<ViewStarBig*>  big_star_list_;
    std::vector<ViewStarBig*>  big_star_free_;
    std::vector<ViewWave*>     wave_list_;
    std::vector<ViewWave*>     wave_free_;

    BackObjectBase::Color color_ = BackObjectBase::Color::kPurple;
};
