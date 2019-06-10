#pragma once

// 板場 温樹

#include "object_base.h"

//-----------------------------------------------------------------------------
// 空間クラス
//-----------------------------------------------------------------------------
// --説明--
// 4分木空間分割法でオブジェクトの衝突判定の無駄をなくす。
// 移動に対応していないので、移動する場合は再登録する( registration関数は連続で呼んでも良い )。
// 
// getInstance関数    : Spaceクラスの実体へのポインタを取得する
// registration関数   : オブジェクトを空間に登録する
// unregistration関数 : オブジェクトを空間から登録解除する
// collision関数      : 空間に所属する全オブジェクトの衝突判定を行う
class Space
{
private:
    // Singleton
    Space();
public:
    ~Space();

    static Space* getInstance() { static Space instance; return &instance;  }


    void registration( ObjectBase* const Object,
                       const Vector2&    Origin,
                       const float       Radius );

    void unregistration( ObjectBase* const Object );


    void collision();

private:
    RECT range_ = { 0L, 0L, 0L, 0L };

    std::vector<std::list<ObjectBase*>> space_;
    std::unordered_map<ObjectBase*, std::list<ObjectBase*>*> object_list_;
};