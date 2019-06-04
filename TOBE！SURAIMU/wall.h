#pragma once

// ”Âê ‰·÷

#include "object_base.h"

struct Line;

//-----------------------------------------------------------------------------
// •Ç
//-----------------------------------------------------------------------------
// --à–¾--
// getShapeŠÖ”‚É“n‚·ˆø”‚Í ¶‚Ì•Ç : 0A‰E‚Ì•Ç : 1B
class Wall :
	public ObjectBase
{
public:
	Wall();
	~Wall();
public:
	virtual bool init();
	virtual void destroy();
	virtual void update();
	virtual void draw();

	virtual Line* getShape(const unsigned GetNum);

private:
    bool created_ = false;

	Line *myshape_;
};
