#pragma once

#include "scene_base.h"

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
	bool is_create_ = true;
	bool create();

	bool(Play::*update_)() = nullptr;
	bool start();
	bool play();
	bool pause();

private:
	StarContainer* star_ = nullptr;
};

