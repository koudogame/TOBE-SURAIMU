#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "scene_base.h"
#include "background_container.h"
#include "back_object_container.h"

class Game
{
public:
	Game();
	~Game();

public:
//外部公開関数
	bool init();
	bool update();
	void draw();
	void destroy();

private:
	std::unique_ptr<SceneBase> scene_;		//stateパターンシーン格納用変数

	std::unique_ptr<BackgroundContainer> background_container_;
	std::unique_ptr<BackObjectContainer> back_object_container_;
};

