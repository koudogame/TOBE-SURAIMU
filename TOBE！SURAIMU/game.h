#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "scene_base.h"
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
	ID3D11ShaderResourceView* texture_;
};

