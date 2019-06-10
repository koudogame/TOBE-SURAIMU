#pragma once

#include "object_base.h"
#include "shape.h"

//-----------------------------------------------------------------------------
// 範囲検索オブジェクト
//-----------------------------------------------------------------------------
// --説明--
// 指定範囲( 円 )の中にある検索対象オブジェクトをリスト化する
// --関数--
// update関数 : リストの更新
//
// set関数 : 原点と半径を設定する( update関数よりも前に呼び出して )
class Sercher :
    public ObjectBase
{
public:
    Sercher() = default;
    virtual ~Sercher() = default;
    Sercher( const Vector2& Origin, const float Radius )
    {
        set( Origin, Radius );
    }

    virtual bool init() override { return true; }
    virtual void update() override = 0;
    void draw() override {}


// セッター
    void set(const Vector2& Origin, const float Radius)
    {
        range_.position = Origin;
        range_.radius = Radius;
    }
    void setOrigin(const Vector2& Origin) { set(Origin, range_.radius); }
    void setRadius(const float Radius) { set(range_.position, Radius); }

// ゲッター


private:
    Circle range_;
};