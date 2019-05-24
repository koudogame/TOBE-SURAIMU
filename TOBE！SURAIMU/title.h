#pragma once

#include "scene_base.h"
#include "title_object.h"
#include "audio_loader.h"

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
	bool next_flag_;

	Menu select_menu_;
	std::unique_ptr<TitleObject> object_[ 3 ];

	AudioContainer* title_bgm_;
	float volume_;

private:
	enum ObjectNum
	{
		kRogo,
		kMenu,
		kCusur
	};

	//内部利用関数
private:
	void input();
};
