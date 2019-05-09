#pragma once

#include "object_container_base.h"

class Star;

//-----------------------------------------------------------------------------
// スターコンテナ
//-----------------------------------------------------------------------------
class StarContainer :
	public ObjectContainerBase<Star>
{
public:
	StarContainer(TaskManager* const TaskManager );
	~StarContainer();
public:
	void update() override;
	void destroy() override;

	void addStar(
		const Vector2& Position,
		const float Angle,
		const float FallSpeed,
		const float SpinSpeed,
		const float SpinRate,
		const float Size
	);
};
