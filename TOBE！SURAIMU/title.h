#pragma once

#include "scene_base.h"
#include "background_container.h"

//-----------------------------------------------------------------------------
// タイトル
//-----------------------------------------------------------------------------
class Title :
	public SceneBase
{
public:
	Title();
	~Title();

	//継承関数
public:
	bool init() override;
	void destroy() override;
	SceneBase* update() override;
	void draw() override;

	//内部利用列挙体
private:
	enum Menu
	{
		kPlay,
		kRanking
	};

	//メンバ変数
private:
	GamePad::ButtonStateTracker pad_;
	GamePad::State pad_state_;
	Keyboard::KeyboardStateTracker key_;

	Menu select_menu_;

	//内部利用関数
private:
	void input();
};
