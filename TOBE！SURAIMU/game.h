#pragma once
#include "scene_base.h"

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
	SceneBase* scene_;		//stateパターンシーン格納用変数
};

