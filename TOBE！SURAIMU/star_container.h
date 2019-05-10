#pragma once

#include "object_container_base.h"

class Star;

//-----------------------------------------------------------------------------
// スターコンテナ
//-----------------------------------------------------------------------------
// --説明--
// スターの追加はaddStar関数を使用する
// setFall関数ですべてのスターに落下を追加する
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
	void setFall();
};
