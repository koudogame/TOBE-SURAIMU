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

//定数
const float kChangeAlphaVol = (kBandMaxAlpha - kBandMinAlpha) / 100.0F;
const float kChangeScaleVol = (kBandMaxScale - kBandMinScale) / 100.0F;
const float kCreateIntervalScale = 0.3F;
const int kWarmUp = 100;


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
	band_inf_.push_back(BandInf(kBandMaxAlpha,kBandMinScale));

	for (int i = 0; i < kWarmUp; ++i)
		this->update();

	return true;
}

void Band::update()
{
	for (auto itr = band_inf_.begin();itr != band_inf_.end();)
	{
		itr->scale += kChangeScaleVol;
		itr->alpha -= kChangeAlphaVol;

		if (itr->scale > kBandMaxScale)
			itr = band_inf_.erase(itr);
		else
			++itr;
	}

	if (band_inf_.back().scale > kCreateIntervalScale)
		band_inf_.push_back(BandInf(kBandMaxAlpha, kBandMinScale));
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
		Sprite::getInstance()->reserveDraw(texture_, DrawPosition, trim, itr.alpha, 0.69F, Vector2(1.0F, itr.scale), 0.0F, Vector2(kBandWidth / 2.0F, kBandHeight), Common::getInstance()->getStates()->Additive());
	}
}

void Band::destroy()
{
	TextureLoder::getInstance()->release(texture_);
}

float Band::getHeight()
{
	return kBandHeight;
}
