#pragma once

#include "scene_base.h"
#include "title_object.h"
#include "timer.h"
#include "csvLoader.h"

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
	//初期化
	bool init() override;
	//破棄
	void destroy() override;
	//更新
	SceneBase* update() override;
	//描画
	void draw() override;

	//内部利用列挙体
private:
	//メニュー
	enum Menu
	{
		kPlay,
		kRanking
	};

	//オブジェクト番号
	enum ObjectNum
	{
		kRogo,
		kCusur,
		kObjectNum
	};

	//メンバ変数
private:
	GamePad::ButtonStateTracker pad_;				                    //パッドトラッカー
	GamePad::State pad_state_;						                    //パッド
	Keyboard::KeyboardStateTracker key_;			                    //キーボード

	Menu select_menu_;									                //メニュー変数
	TitleStatus object_status_[ObjectNum::kObjectNum];	                //オブジェクト情報
	float volume_;				                                        //音量
	SceneBase* (Title::* scene_)() = nullptr;		                    //更新処理の関数ポインタ
	Timer<Seconds> timer_;
	CsvLoader first_stage_;

	//内部利用関数
private:
	//入力
	void input();
	//ゲーム部への遷移処理
	SceneBase* playScene();
	//ランキングへの遷移処理
	SceneBase* rankingScene();
	//選択部の処理
	SceneBase* selectScene();
	//デモへの遷移
	SceneBase* DemoScene();

};
