
// 板場 温樹

#include "back_object_container.h"


/*===========================================================================*/
BackObjectContainer::BackObjectContainer()
{
}

BackObjectContainer::~BackObjectContainer()
{
}

/*===========================================================================*/
// 更新処理
void BackObjectContainer::update()
{
    // 死亡判定後のオブジェクトのdestroy関数を呼ばない( 後にreset関数で値を再設定する )
    for (auto itr = active_list_.begin(), end = active_list_.end();
        itr != end;)
    {
        if ((*itr)->isAlive() == false)
        {
            free_list_.push_back(*itr);
            itr = active_list_.erase(itr);
        }
        else { ++itr; }
    }
}

/*===========================================================================*/
// 背景オブジェクトの追加
bool BackObjectContainer::addBackObject(const RECT& Trimming,
                                        const float ScrollX, const float ScrollY,
                                        const float DrawDepth)
{
    BackObject* back_obj = getFreeObject();

    // 空きがなかったら新規生成
    if (back_obj == nullptr)
    {
        back_obj = getFreeObjAndInsert();
        if (back_obj->init(
            Trimming,
            ScrollX, ScrollY,
            DrawDepth) == false)
        {
            return false;
        }
    }
    // 空きがあったら使いまわす
    else
    {
        back_obj->reset(
            Trimming,
            ScrollX,
            ScrollY,
            DrawDepth);
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

/*===========================================================================*/
// フリーリストに要素があったらそれを返却
BackObject* BackObjectContainer::getFreeObject()
{
    // 要素が無い
    if (free_list_.size() == 0) { return nullptr; }

    BackObject* free = free_list_.back();
    free_list_.pop_back();
    return free;
}
