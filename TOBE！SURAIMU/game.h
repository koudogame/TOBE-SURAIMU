#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "scene_base.h"
#include "back_object_container.h"
#include "background.h"

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
	Background background_;
    std::unique_ptr<BackObjectContainer> backobject_container_;
	ID3D11ShaderResourceView* texture_;
};

