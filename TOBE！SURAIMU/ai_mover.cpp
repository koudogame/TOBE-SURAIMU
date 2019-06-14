
#include "ai_mover.h"

#include "Sound.h"
#include "release.h"
#include "calc.h"
#include "sercher.h"


/*===========================================================================*/
#define TODEGREES( Radians )    (Radians) * kToDegrees
#define TORADIANS( Degrees )    (Degrees) * kToRadians

constexpr float kToDegrees = 180.0F / XM_PI;
constexpr float kToRadians = XM_PI / 180.0F;
constexpr float kSerchRange   = 300.0F;
constexpr float kJumpAngleMin = 45.0F;
constexpr float kJumpAngleMax = 135.0F;
constexpr float kSpeed = 5.0F;
constexpr int kBottomOff = 1;
constexpr int kBottomOn  = 3;

/*===========================================================================*/
AIMover::AIMover()
{

}

AIMover::~AIMover()
{

}

/*===========================================================================*/
// 初期化処理
bool AIMover::init( const Vector2& Position,    // 初期座標
                    const int PlayerNo )        // プレイヤー番号
{
    Player::init( Position, PlayerNo );

    sercher_ = new Sercher();
    if( sercher_->init(Position, kSerchRange, ObjectID::kStar) == false )
    {
        return false;
    }

    purpose_ = nullptr;
    jump_angle_ = getRandJumpAngle();

    return true;
}

/*===========================================================================*/
// 終了処理
void AIMover::destroy()
{
    if( sercher_ != nullptr )
    {
        sercher_->destroy();
        safe_delete( sercher_ );
    }

    Player::destroy();
}

/*===========================================================================*/
// 更新処理
void AIMover::update()
{
    Player::update();

    sercher_->setOrigin( myshape_.position );
    sercher_->update();
}


/*===========================================================================*/
// 入力処理
void AIMover::input( float Sin )
{
    const float kAngle = TODEGREES(revition_angle_ + XM_PI);

    // 接地中
    if( !flag_.test(kJump)) {
        direction_id_ = Direction::kFlont;
        movement_angle_ = revition_angle_;
        
        purpose_ = nullptr;

        //ジャンプ                           /*誤差*/
        if( std::abs(kAngle - jump_angle_) <= 5.0F &&
            !died_flag_)
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
            base_angle_ = movement_angle_ = movement_angle_ + XM_PI;
            ground_ = &kGround;
            prev_jump_moveamount_ = 0;
            score_.resetRotate();

            // 次のジャンプ角度を設定する
            jump_angle_ = getRandJumpAngle();
        }
    }
    // 目的を設定する
    else if( purpose_ == nullptr )
    {
        auto purposes = sercher_->getList();
        const size_t purposes_num = purposes.size();

        if( purposes_num > 0U ){
            purpose_ = purposes[ rand() % purposes_num ];
        }
    }
    // 目的へ移動する
    else
    {
        Vector2 kPurPosi = purpose_->getPosition();
        if( kPurPosi.y > getWindowHeight<float>() )
        {
            purpose_ = nullptr;
            return;
        }

        const float kDistance = Calc::magnitude( kPurPosi - myshape_.position );

        // 目的地との距離が大きいかつ、自分が上にいたら下移動
        if( kDistance > 300.0F  )
        {
            if( kPurPosi.y > myshape_.position.y )
            {
                bottom_input_ = kBottomOn;
                score_.addDown();
                score_.resetCombo();
                flag_.reset(Flag::kStarCollision);
            }
            else
            {
                bottom_input_ = kBottomOff;
            }
        }
        // 目的地との距離が小さいかつ、横に離れていたら横移動
        else
        {
            // 右移動
            if( (kPurPosi.x - myshape_.position.x) > 20.0F )
            {
                movement_angle_ -= TORADIANS( kSpeed );
            }
            // 左移動
            else if( (kPurPosi.x - myshape_.position.x) < 20.0F )
            {
                movement_angle_ += TORADIANS( kSpeed );
            }
        }
    }
}

/*===========================================================================*/
// ランダムな角度( ジャンプ可能範囲内 )を返却
float AIMover::getRandJumpAngle()
{
    return rand() % 
           (static_cast<int>(kJumpAngleMax - kJumpAngleMin)) + 
           kJumpAngleMin;
}