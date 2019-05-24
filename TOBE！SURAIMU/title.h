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
	std::unique_ptr<TitleObject> object_[ 2 ];

	AudioContainer* title_bgm_;
	AudioContainer* select_se_[2];
	AudioContainer* scene_se_;
	float volume_;
	float alpha_;
	SceneBase* ( Title::* scene_ )( ) = nullptr;

private:
	enum ObjectNum
	{
		kRogo,
		kCusur
	};

	//内部利用関数
private:
	void input();
	SceneBase* playScene();
	SceneBase* rankingScene();
	SceneBase* selectScene();

};
