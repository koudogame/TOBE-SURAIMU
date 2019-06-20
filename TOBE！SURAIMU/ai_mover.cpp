
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
constexpr float kJumpAngleMin = 70.0F;
constexpr float kJumpAngleMax = 110.0F;
constexpr float kMaxOffset = 3.0F;
constexpr float kSpeed = 0.1F;
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
    if( purpose_ != nullptr &&
        ((purpose_->isAlive() == false) ||
         (purpose_ == old_owner_)) )
    {
        purpose_ = nullptr;
    }

    Player::update();

    float angle = std::atan2( -movement_.y, movement_.x );

    // 上向き
    if( TODEGREES(angle) < 180.0F )
    {
        sercher_->setOrigin( myshape_.position );
    }
    // 下向き
    else
    {
        old_owner_ = nullptr;
        purpose_ = nullptr;
        sercher_->setOrigin( {myshape_.position.x, myshape_.position.y + kSerchRange} );
    }
}

/*===========================================================================*/
// 入力処理
void AIMover::inputjump()
{
    const float kAngle = TODEGREES(revision_angle_ + XM_PI);

    direction_id_ = Direction::kFlont;

        

    //ジャンプのタイミング                 /*誤差*/
    if( std::abs(kAngle - jump_angle_) <= 5.0F &&
        !died_flag_)
    {
        // ジャンプ
        old_owner_ = owner_;
        SOUND->stop ( SoundId::kJump );
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
        prev_jump_moveamount_ = 0;
        particle_alpha_ = 1.0F;
        score_.resetRotate();

        // 次のジャンプ角度を設定する
        jump_angle_ = getRandJumpAngle();
    }
}

void AIMover::inputmove()
{
    // 目的の設定
    const auto kPurposes = sercher_->getList();
    const size_t kPurposesNum = kPurposes.size();
        
#if 1
    float dist_to_pur = kSerchRange;
    float temp = 0.0F;
    for( size_t i = 0; i < kPurposesNum && purpose_ == nullptr; ++i )
    {
        temp = Calc::magnitude(kPurposes[i]->getPosition() - myshape_.position);
        if( temp < dist_to_pur )
        {
            purpose_ = kPurposes[i];
            dist_to_pur = temp;
        }
    }
#else
    for( size_t i = 0; i < kPurposesNum && purpose_ == nullptr; ++i )
    {
        purpose_ = kPurposes[ rand() % kPurposesNum ];

        // 前回のオーナーは目的としない
        if( purpose_ == owner_ )
        {
             purpose_ = nullptr;
        }
    }
#endif


    if( purpose_ != nullptr )
    {
        Vector2 kPurPosi = purpose_->getPosition();


        // 目的との座標のずれ(x座標)がないかつ、
        // 目的が下にあったら下移動
        if( (std::abs(purpose_->getPosition().x - myshape_.position.x) < 30.0F) &&
            (purpose_->getPosition().y > myshape_.position.y) )
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

        // 目的地との距離が小さいかつ、横に離れていたら横移動
        if( std::abs(purpose_->getPosition().y - myshape_.position.y) < 300.0F )
        {
            const float kAngle = std::atan2( -movement_.y, movement_.x );
            Vector2 temp;

            // 右移動
            if ((kPurPosi.x - myshape_.position.x) > 20.0F)
            {
                if (std::cos(kAngle - XM_PIDIV2) > 0)
                    temp = Vector2(std::cos(kAngle - XM_PIDIV2), -std::sin(kAngle - XM_PIDIV2))*kSpeed;
                else
                    temp = -Vector2(std::cos(kAngle - XM_PIDIV2), -std::sin(kAngle - XM_PIDIV2))*kSpeed;
            }
            // 左移動
            else if ((kPurPosi.x - myshape_.position.x) < -20.0F)
            {
                if (std::cos(kAngle + XM_PIDIV2) < 0)
                    temp = Vector2(std::cos(kAngle + XM_PIDIV2), -std::sin(kAngle - XM_PIDIV2))*kSpeed;
                else
                    temp = -Vector2(std::cos(kAngle + XM_PIDIV2), -std::sin(kAngle + XM_PIDIV2))*kSpeed;
            }
            if (Vector2(offset_ + temp).Length() < kMaxOffset)
                offset_ += temp;
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