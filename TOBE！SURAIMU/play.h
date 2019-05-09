#pragma once

#include "scene_base.h"

class TaskManager;
class StarContainer;

//-----------------------------------------------------------------------------
// ƒvƒŒƒC•”
//-----------------------------------------------------------------------------
class Play :
	public SceneBase
{
public:
	Play();
	~Play();
public:
	bool init() override;
	void destroy() override;
	SceneBase* update() override;
	void draw() override;

private:
	bool do_create_ = true;
	bool create();

	FILE* star_pattern_file_ = nullptr;
	bool setStarPattern();
	bool createStar();

	SceneBase*(Play::*update_)() = nullptr;
	SceneBase* start();
	SceneBase* play();
	SceneBase* pause();

private:
	TaskManager* task_manager_ = nullptr;
	StarContainer* star_container_ = nullptr;
};
