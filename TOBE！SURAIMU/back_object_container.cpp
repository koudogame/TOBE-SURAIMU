
// 板場 温樹

#include "back_object_container.h"


/*===========================================================================*/
BackObjectContainer::BackObjectContainer(TaskManager * const TaskManager) :
    ObjectContainerBase(TaskManager)
{
}

BackObjectContainer::~BackObjectContainer()
{
}

/*===========================================================================*/
// 背景オブジェクトの追加
bool BackObjectContainer::addBackObject(const RECT& Trimming,
                                   const float ScrollX, const float ScrollY,
                                   const float DrawDepth)
{
    BackObject* back_obj = getFreeObjAndInsert();

    if (back_obj->init(
        L"Texture/background.png",
        Trimming,
        ScrollX, ScrollY,
        DrawDepth) == false)
    {
        return false;
    }

    return true;
}

/*===========================================================================*/
// y座標の移動( スクロール )
void BackObjectContainer::setMove(const float Move)
{
    for (auto& elem : active_list_)
    {
        elem->setMove(Move);
    }
}
