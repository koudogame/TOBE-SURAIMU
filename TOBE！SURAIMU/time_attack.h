#pragma once

#include "scene_base.h"
#include "numbers.h"

class TaskManager;
class StarContainer;
class Player;
class Wall;

//-----------------------------------------------------------------------------
// タイムアタック
//-----------------------------------------------------------------------------
class TimeAttack :
	public SceneBase
{
public:
	TimeAttack();
	~TimeAttack();
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

	SceneBase*(TimeAttack::*update_)() = nullptr;
	SceneBase* start();
	SceneBase* play();
	SceneBase* pause();

	Numbers<int> score_;


	ID3D11ShaderResourceView* texture_numbers_;
	std::chrono::high_resolution_clock::time_point prev_time_;
	Numbers<long long> remaining_time_sec_;

private:
	TaskManager* task_manager_ = nullptr;
	StarContainer* star_container_ = nullptr;
	Player* player_ = nullptr;
	Wall* wall_ = nullptr;
};
