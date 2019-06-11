#pragma once

#include "object_base.h"

#include "shape.h"

//-----------------------------------------------------------------------------
// 範囲検索オブジェクト
//-----------------------------------------------------------------------------
// --説明--
// 指定範囲( 円 )の中にある検索対象オブジェクトをリスト化する
// --関数--
// init関数 : 初期化処理
// -Origin = 検索範囲原点
// -Radius = 検索範囲半径
//
// update関数 : 空間への再登録とリストの更新
//
// collision関数 : 検索範囲に衝突したオブジェクトとの衝突処理
class Sercher :
    public ObjectBase
{
public:
    Sercher() = default;
    virtual ~Sercher();
    Sercher( const Vector2& Origin, const float Radius,
             const ObjectID Target );

    virtual bool init( const Vector2& Origin, const float Radius,
                       const ObjectID Target = kNone);

    virtual void destroy() override;

    virtual void update() override;
    virtual void draw() override {}

    void collision( ObjectBase* const Object );

// セッター
    void setRange( const Vector2& Origin, const float Radius )
        {
            range_.position = Origin;
            range_.radius = Radius;
        }
    void setOrigin( const Vector2& Origin ) { setRange(Origin, range_.radius); }
    void setRadius( const float Radius )    { setRange(range_.position, Radius); }
    void setTarget( const ObjectID Target ) { target_ = Target; }

// ゲッター
    const Circle&  getRange()  const { return range_; }
    const Vector2& getOrigin() const { return getRange().position; }
          float    getRadius() const { return getRange().radius; }
          ObjectID getTarget() const { return target_; }
    const std::deque<ObjectBase*>& getList() const { return collision_list_; }

private:
    Circle range_ { {0.0F, 0.0F}, 0.0F };
    ObjectID target_ = kNone;
    std::deque<ObjectBase*> collision_list_;
};