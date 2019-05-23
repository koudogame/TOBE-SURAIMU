#pragma once

// 板場 温樹

#include "object_container_base.h"
#include "background.h"

//-----------------------------------------------------------------------------
// 背景コンテナ
//-----------------------------------------------------------------------------
// --説明--
// addBackground関数で要素の追加を行う
// Trimming  : 画像の切り取り範囲
// Scroll    : スクロール速度
// DrawDepth : 描画深度
class BackgroundContainer : 
    public ObjectContainerBase<Background>
{
public :
    BackgroundContainer();
    ~BackgroundContainer();

    bool addBackground(const RECT& Trimming,
                       const float Scroll = 1.0F,
                       const float DrawDepth = 0.0F);

    void setMove(const float MoveY);
};
