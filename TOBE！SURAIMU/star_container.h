#pragma once

#include "object_container_base.h"


//-----------------------------------------------------------------------------
// スターコンテナ
//-----------------------------------------------------------------------------
class StarContainer :
	public ObjectContainerBase
{
public:
	StarContainer(TaskManager* const TaskManager );
	~StarContainer();
public:
	// 星の追加
	void addStar(
		const Vector2& Position,
		const float Angle,
		const float FallSpeed,
		const float SpinSpeed,
		const float SpinRate,
		const float Size
	);
};
