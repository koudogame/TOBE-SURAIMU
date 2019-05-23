#pragma once

// 板場 温樹

#include "scene_base.h"

struct Seconds;
template <typename T>
class Timer;
template <typename T>
class Numbers;
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

	Numbers<int>* score_;
	float player_rotate_sum_;
	void scoring();
private:
	ID3D11ShaderResourceView* texture_numbers_;
	Timer<Seconds>* timer_ = nullptr;
	StarContainer* star_container_ = nullptr;
	Player* player_ = nullptr;
	Wall* wall_ = nullptr;
};
