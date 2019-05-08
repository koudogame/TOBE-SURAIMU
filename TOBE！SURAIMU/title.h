
#include "scene_base.h"

//-----------------------------------------------------------------------------
// ƒ^ƒCƒgƒ‹
//-----------------------------------------------------------------------------
class Title :
	public SceneBase
{
public:
	Title();
	~Title();
public:
	bool init() override;
	void destroy() override;
	SceneBase* update() override;
	void draw() override;

private:

};
