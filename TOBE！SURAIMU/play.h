#pragma once

#include "scene_base.h"

class TaskManager;
class StarContainer;
class Player;

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

	std::deque<std::string> star_pattern_list_;
	FILE* star_pattern_ = nullptr;
	bool createStar();

	SceneBase* start();
	SceneBase* play();
	SceneBase* pause();
	SceneBase*(Play::*update_)() = nullptr;

private:
	TaskManager* task_manager_ = nullptr;
	StarContainer* star_container_ = nullptr;
	Player* player_ = nullptr;
};
