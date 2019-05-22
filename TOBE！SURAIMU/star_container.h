#pragma once

// 板場 温樹

#include "object_container_base.h"
#include "star.h"

//-----------------------------------------------------------------------------
// スターコンテナ
//-----------------------------------------------------------------------------
// --説明--
// スターの追加はaddStar関数を使用する( 生成失敗でreturn nullptr )
// setFall関数ですべてのスターに落下を追加する
class StarContainer :
	public ObjectContainerBase<Star>
{
public:
	StarContainer(TaskManager* const TaskManager );
	~StarContainer();
public:
	Star* addStar(
		const Vector2& Position,
		const float Angle,
		const float SpinSpeed,
		const float SpinRate,
		const float Size
	);

    void resetStates(const float Magnification);
    void setMove(const float MoveY);
};
