#pragma once

// 板場 温樹

#include "object_container_base.h"
#include "back_object.h"

//-----------------------------------------------------------------------------
// 背景オブジェクトコンテナ
//-----------------------------------------------------------------------------
// --説明--
// addBackObject関数で要素の追加を行う
// Trimming  : 画像の切り取り範囲
// ScrollX   : X方向のスクロール量
// ScrollY   : Y方向のスクロール量
// DrawDepth : 描画深度
class BackObjectContainer :
    public ObjectContainerBase<BackObject>
{
public:
    BackObjectContainer();
    ~BackObjectContainer();

    void update() override;

    bool addBackObject(
        const RECT& Trimming,
        const float ScrollX = 1.0F,
        const float ScrollY = 1.0F,
        const float DrawDepth = 0.0F);

    void setMove(const float MoveY);
    bool empty() { return active_list_.size() == 0; }

private:
    BackObject* getFreeObject();
};