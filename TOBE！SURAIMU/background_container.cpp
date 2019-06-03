
// 板場 温樹

#include "background_container.h"


/*===========================================================================*/
BackgroundContainer::BackgroundContainer()
{
}

BackgroundContainer::~BackgroundContainer()
{
}

/*===========================================================================*/
// 背景の追加
// Trimming : 画像切り取り範囲
// Scroll   : y座標の1フレームでのスクロール量
// Depth    : Sprite深度値
bool BackgroundContainer::addBackground(const RECT& Trimming,
                                        const float Scroll,
                                        const float DrawDepth)
{
    Background* background = getFreeObjAndInsert();

    if( background->init(
        Trimming,
        Scroll,
        DrawDepth
    ) == false)
    {
        return false;
    }


    return true;
}

/*===========================================================================*/
// y座標の移動( スクロール )
void BackgroundContainer::setMove(const float Move)
{
    for (auto& elem : active_list_)
    {
        elem->setMove(Move);
    }
}
