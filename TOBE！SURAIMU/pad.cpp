
#include "pad.h"

void Pad::update()
{
	// 入力を取得＆トラッカー更新
	for (int i = 0; i < 4; i++)
	{
		state_[i] = pad_.GetState(i);
		tracker_[i].Update(state_[i]);
	}

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

//**************************************************************************//
// 処理内容	: 指定された秒数ゲームパッド振動
// 引数リスト	: 振動継続時間( 秒 )
// 戻り値			: 無し
void Pad::Vibration( float Second, float Right, float Left )
{
    second_ = Second;
    vib_right_ = Right;
    vib_left_ = Left;

    flag_ = true;
}