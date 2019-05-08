#include "container.h"
#include "release.h"

//コンストラクタ
Container::Container(ID3D11ShaderResourceView * Texture) :
	texture_(Texture),
	ref_cnt_(0)
{
}

//デストラクタ
Container::~Container()
{
	SAFE_RELEASE(texture_);
}
