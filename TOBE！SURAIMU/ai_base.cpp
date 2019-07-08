
// 板場　温樹

#include "ai_base.h"

#include "Sound.h"

/*===========================================================================*/
constexpr int kMoveLeft  = 0;
constexpr int kMoveRight = 1;
constexpr int kMoveDown  = 2;
constexpr float kSpeed   = 0.5F;
constexpr int kBottomOn  = 3;		                //下入力時の重力倍率
constexpr int kBottomOff = 1;		                //下入力なしの重力倍率
constexpr float kMaxOffset = 5.0F;


/*===========================================================================*/
void AIBase::inputjump()
{
    if( flag_.test(kJump) == false )
    {
        // ジャンプ
        if( isJump() )       { jump(); }
        // しゃがむ
        else if( isSquat() ) { direction_id_ = Direction::kSquat; } 
    }
}

void AIBase::inputmove()
{
    if( isMoveLeft() )
    {
    // 左移動
        move( kMoveLeft );
    }
    if( isMoveRight() )
    {
    // 右移動
        move( kMoveRight );
    }

    if( isMoveDown() )
    {
    // 下移動
        move( kMoveDown );
    }
    else
    {
    // 下移動無し
        bottom_input_ = kBottomOff;
    }
}

/*===========================================================================*/
void AIBase::jump()
{
    SOUND->stop(SoundId::kJump);
    SOUND->play(SoundId::kJump, false);
    flag_.set(Flag::kJump);
    flag_.set(Flag::kStarCollision);
    flag_.reset(Flag::kParticle);
    flag_.reset(Flag::kOnce);
    direction_id_ = Direction::kFlay;
    particle_time_ = 0;
    now_amount_ = 0.0F;
    base_angle_ = revision_angle_ + XM_PI;
    ground_ = &kGround;
    prev_jump_moveamount_ = 0.0F;
    score_.resetRotate();
}

void AIBase::move( int Way )
{
    Vector2 move = movement_ + offset_;
    float angle = std::atan2(-move.y, move.x);
    Vector2 temp = Vector2::Zero;
    switch ( Way )
    {
    // 左移動
    case kMoveLeft:
        if (std::cos(angle + XM_PIDIV2) < 0)
        {
            temp.x = std::cos(angle + XM_PIDIV2) * kSpeed;
            temp.y = std::sin(angle - XM_PIDIV2) * -kSpeed;
        }
        else
        {
            temp.x = std::cos( angle + XM_PIDIV2 ) * -kSpeed;
            temp.y = std::sin( angle + XM_PIDIV2 ) * kSpeed;
        }

        if( Vector2(offset_ + temp).Length() < kMaxOffset )
        {
            offset_ += temp;
        }
        break;

    // 右移動
    case kMoveRight:
        if (std::cos(angle - XM_PIDIV2) > 0)
        {
            temp.x = std::cos( angle - XM_PIDIV2 ) * kSpeed;
            temp.y = std::sin( angle - XM_PIDIV2 ) * -kSpeed;
        }
        else
        {
            temp.x = std::cos( angle - XM_PIDIV2 ) * -kSpeed;
            temp.y = std::sin( angle - XM_PIDIV2 ) * kSpeed;
        }

        if (Vector2(offset_ + temp).Length() < kMaxOffset)
        {
            offset_ += temp;
        }
        break;

    // 下移動
    case kMoveDown:
        bottom_input_ = kBottomOn;
        score_.addDown();
        score_.resetCombo();
        flag_.reset(Flag::kStarCollision);

        break;
    }
}
