#pragma once

#include "scene_base.h"
#include "title_object.h"

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

	enum ObjectNum
	{
		kRogo ,
		kCusur ,
		kStar1 ,
		kStar2 ,
		kPlayer ,
		kWallRight,
		kWallLeft,
		kObjectNum
	};

	//メンバ変数
private:
	GamePad::ButtonStateTracker pad_;
	GamePad::State pad_state_;
	Keyboard::KeyboardStateTracker key_;
	bool next_flag_;

	Menu select_menu_;
	TitleStatus object_status_[ ObjectNum::kObjectNum ];
	std::unique_ptr<TitleObject> object_[ ObjectNum::kObjectNum ];

	float volume_;
	SceneBase* ( Title::* scene_ )( ) = nullptr;

	//内部利用関数
private:
	void input();
	SceneBase* playScene();
	SceneBase* rankingScene();
	SceneBase* selectScene();

};
