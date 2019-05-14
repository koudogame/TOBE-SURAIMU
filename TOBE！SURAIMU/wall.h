#pragma once

#include "object_base.h"

struct Line;

//-----------------------------------------------------------------------------
// 壁
//-----------------------------------------------------------------------------
// --説明--
// getShape関数に渡す引数は 左の壁 : 0、右の壁 : 1。※他の数字を渡したら「0」として扱う
class Wall :
	public ObjectBase
{
public:
	Wall(TaskManager* const TaskManager);
	~Wall();
public:
	virtual bool init();
	virtual void destroy();
	virtual void update();
	virtual void draw();

	virtual Line* getShape(const unsigned GetNum);

public:
	bool do_create_ = true;
	bool create();

private:
	Line *myshape_;
};
