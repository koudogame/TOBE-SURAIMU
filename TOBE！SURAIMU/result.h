#pragma once

#include "scene_base.h"

//-----------------------------------------------------------------------------
// ƒŠƒUƒ‹ƒg
//-----------------------------------------------------------------------------
class Result :
	public SceneBase
{
public:
	Result();
	~Result();
public:
	bool init() override;
	void destroy() override;
	SceneBase* update() override;
	void draw() override;

private:

};
