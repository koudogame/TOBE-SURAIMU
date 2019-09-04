
#include "pad.h"

void Pad::update()
{
	// 入力を取得＆トラッカー更新
	state_ = pad_.GetState(0);
	tracker_.Update(state_);

	// 振動
	if (flag_)
	{
		if ((count_ += 1.0F) <= (second_ * 60.0F))
		{
			pad_.SetVibration(0, 0.5F, 0.5F);
		}
		else
		{
			count_ = 0.0F;
			flag_ = false;
			pad_.SetVibration(0, 0.0F, 0.0F);
		}
	}
}

void Pad::update( Dinput * const Instance)
{
	if ((*Instance->getDevice()) == nullptr)
		state_ = pad_.GetState(0);
	else
		Instance->update(&state_);
	tracker_.Update(state_);
}

//**************************************************************************//
// 処理内容	: 指定された秒数ゲームパッド振動
// 引数リスト	: 振動継続時間( 秒 )
// 戻り値			: 無し
void Pad::Vibration(float Second, float Right, float Left)
{
	second_ = Second;
	vib_right_ = Right;
	vib_left_ = Left;

	flag_ = true;
}