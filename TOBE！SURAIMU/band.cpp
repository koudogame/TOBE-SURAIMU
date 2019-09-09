#include "stdafx.h"
#include "band.h"
#include "textureLoder.h"
#include "sprite.h"

//ステータス
const float kBandMinAlpha = 0.0F;
const float kBandMaxAlpha = 1.0F;
const float kBandMinScale = 0.0F;
const float kBandMaxScale = 1.5F;
const float kBandWidth = 100.0F;
const float kBandHeight = 300.0F;
//生成本数
const int kCreateBandNum = 5;

//定数
const float kChangeAlphaVol = kBandMaxAlpha / 100.0F;
const float kChangeScaleVol = kBandMaxScale / 100.0F;


Band::Band()
{
}


Band::~Band()
{
}

bool Band::init(int ColorID, int SizeID)
{
	texture_ = TextureLoder::getInstance()->load(L"Texture/band.png");
	color_id_ = ColorID;
	size_id_ = SizeID;
	for (int i = 0; i < kCreateBandNum; ++i)
	{
		band_inf_.push_back(BandInf((rand_() % static_cast<int>(kBandMaxAlpha * 10)) / 10.0F, (rand_() % static_cast<int>(kBandMaxScale * 10)) / 10.0F, rand_() % 2 ? -1 : 1, rand_() % 2 ? -1 : 1));
	}
	return true;
}

void Band::update()
{
	for (auto& itr : band_inf_)
	{
		itr.scale += itr.signal_scale * kChangeScaleVol;
		if (itr.signal_scale > 0)
			itr.alpha -= kChangeAlphaVol;
		else
			itr.alpha = 0;

		if (itr.scale > kBandMaxScale || itr.scale < kBandMinScale)
		{
			itr.signal_scale *= -1;
			if (itr.signal_scale > 0)
				itr.alpha = 1.0F;
		}

	}
}

void Band::draw(const Vector2& DrawPosition)
{
	for (auto& itr : band_inf_)
	{
		RECT trim;
		trim.left = kBandWidth * size_id_;
		trim.top = kBandHeight * color_id_;
		trim.right = trim.left + kBandWidth;
		trim.bottom = trim.top + kBandHeight;
		Sprite::getInstance()->reserveDraw(texture_, DrawPosition, trim, itr.alpha, 0.69F, Vector2(1.0F, itr.scale), 0.0F, Vector2(kBandWidth / 2.0F, kBandHeight));
	}
}

void Band::destroy()
{
	TextureLoder::getInstance()->release(texture_);
}
