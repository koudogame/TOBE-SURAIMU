
// 基底クラス作成者 : 山田 晴久
// 板場 温樹

#include "player_demo.h"

#include "calc.h"
#include "Sound.h"
#include "space.h"


constexpr float kSpeed = 5.0F;              // プレイヤー速さ
constexpr float kJumpTimming = 400.0F;      // ジャンプを入力する高さ
constexpr float kJumpAngleMin = -20.0F;
constexpr float kJumpAngleMax = 20.0F;
constexpr float kSquatAngleMin = -45.0F;
constexpr float kSquatAngleMax = 45.0F;

/*===========================================================================*/
// インプットの上書き
void DemoPlayer::input()
{
    if( flag_.test(kJump) == false )
    {
        // 目標オブジェクトがない場合かつ、ジャンプ位置になったら
        if( purpose_ == nullptr && position_.y > kJumpTimming )
        {
            // 目標オブジェクトを設定する
            purpose_ = serch();
        }

        // 目標オブジェクトがある場合、お互いの角度が一定の範囲内だったら
        float angle = Calc::angle( purpose_->getPosition(), myshape_.position );
        if( purpose_ != nullptr  )
        {
            if( angle >= kJumpAngleMin && angle <= kJumpAngleMax )
            {
                SOUND->stop(SoundId::kJump);
                SOUND->play(SoundId::kJump, false);
                flag_.set(Flag::kJump);
                flag_.set(Flag::kStarCollision);
                flag_.reset(Flag::kParticle);
                direction_id_ = Direction::kFlay;
                particle_time_ = 0;
                now_amount_ = 0.0F;
                jumping_angle_ = gravity_angle_ + XM_PI;
                ground_ = &kGround;
                prev_jump_moveamount_ = 0;
                score_.resetRotate();
            }
            else if( angle >= kSquatAngleMin && angle <= kSquatAngleMax )
            {
                direction_id_ = Direction::kSquat;
            }
        }
    }
    else
    {

    }
}

/*===========================================================================*/
// 近くにある検索対象オブジェクトを検索
ObjectBase* DemoPlayer::serch()
{
    ObjectBase* purpose = Space::getInstance()->
        judgeCollision( 
            &serch_range_,
            serch_range_.getShape()->position,
            serch_range_.getShape()->radius,
            ObjectID::kStar);

    if( purpose == owner_ ) { purpose = nullptr; }



    return purpose;
}
