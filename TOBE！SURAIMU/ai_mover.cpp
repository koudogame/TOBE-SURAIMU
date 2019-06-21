
#include "ai_mover.h"

#include "Sound.h"
#include "release.h"
#include "calc.h"
#include "sercher.h"

using namespace Calc;

/*===========================================================================*/
#define TODEGREES( Radians )    (Radians) * kToDegrees
#define TORADIANS( Degrees )    (Degrees) * kToRadians

constexpr float kWindowWidth = getWindowWidth<float>();
constexpr float kWindowHeight = getWindowHeight<float>();
constexpr float kToDegrees = 180.0F / XM_PI;
constexpr float kToRadians = XM_PI / 180.0F;
constexpr float kSerchRange   = 500.0F;
constexpr float kDeltaJumpAngle = 20.0F;
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

    jumping_ = false;
    jump_angle_ = 500.0F;

    setPurpose();
    setJumpAngle();

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
    if( (purpose_ != nullptr) &&
        (purpose_->isAlive() == false) )
    {
        purpose_ = nullptr;
    }

    Player::update();
    setPurpose();
    setJumpAngle();

    float angle = std::atan2( -movement_.y, movement_.x );


    // ジャンプ中のスター検索範囲設定
    if( flag_.test( kJump ) )
    {
        // 上向き
        if( TODEGREES(angle) < 180.0F )
        {
            sercher_->setOrigin( myshape_.position );
        }
        // 下向き
        else
        {
            sercher_->setOrigin( {myshape_.position.x, myshape_.position.y + kSerchRange} );
        }
    }
    // 接地中のスター検索範囲設定
    else
    {
        // 着地した瞬間に、目的をリセットする
        if( jumping_ )
        {
            jumping_ = false;
            purpose_ = nullptr;
        }

        float angle = revision_angle_ + XM_PI;

        sercher_->setOrigin(
            {
              myshape_.position.x + std::cos( angle ) * kSerchRange,
              myshape_.position.y - std::sin( angle ) * kSerchRange
            } );
    }
}

/*===========================================================================*/
// 入力処理
void AIMover::inputjump()
{
    auto judge = [this]()->bool
    {
        float angle = TODEGREES(revision_angle_ + XM_PI);
        // 画面内にいる
        if( myshape_.position.y < kWindowHeight )
        {
            if( (std::abs(jump_angle_ - angle) <= kDeltaJumpAngle) && // 目的との角度が誤差内
                (angle >= 30.0F && angle <= 150.0F) &&                // 上向き
                !(rand() % 7)                                         // n分の1の確立
              )
            {
                return true;
            }
        }
        // 画面外へ出てしまっている( 焦る )
        else
        {
            if( angle >= 70.0F && angle <= 110.0F )
            {
                return true;
            }
        }

        return false;
    };


    direction_id_ = Direction::kFlont;

    //ジャンプのタイミング
    if( judge() &&
        !died_flag_)
    {
        // ジャンプ
        jumping_ = true;
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
    }
}

void AIMover::inputmove()
{
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
// 目的のスターを設定する
void AIMover::setPurpose()
{
    auto check = [this]( ObjectBase* Target )->bool
    {
        if( Target->isAlive() == false )
        {
            return false;
        }

        // 空中
        if( flag_.test(kJump) )
        {
            float angle = TODEGREES( std::atan2(-movement_.y, movement_.x) );

            // 上向き
            if( angle < 180.0F )
            {
                if( Target == owner_ )
                {
                    return false;
                }
            }
            // 下向き
            else
            {
                if( Target->getPosition().y < myshape_.position.y )
                {
                    return false;
                }
            }
        }
        // 接地
        else 
        {
            if( Target == owner_ ||
                Target->getPosition().y > myshape_.position.y)
            {
                return false;
            }
        }

        return true;
    };


    const auto kPurposes = sercher_->getList();
    const size_t kPurposesNum = kPurposes.size();
    float dist_to_pur;
    if( purpose_ != nullptr )
    {
        dist_to_pur = magnitude( purpose_->getPosition() - myshape_.position);
    }
    else
    {
        dist_to_pur = kSerchRange;
    }

    float temp = 0.0F;
    for (size_t i = 0; i < kPurposesNum; ++i)
    {
        if( kPurposes[i]->getPosition().y < kWindowHeight &&
            check( kPurposes[i]) )
        {
        temp = magnitude(kPurposes[i]->getPosition() - myshape_.position);
        if( temp < dist_to_pur )
        {
            purpose_ = kPurposes[i];
            dist_to_pur = temp;
        }
        }
    }
}
// ジャンプの角度を設定
void AIMover::setJumpAngle()
{
    if( purpose_ != nullptr )
    {
        jump_angle_ = angle( purpose_->getPosition() - myshape_.position );
        jump_angle_ = TODEGREES( jump_angle_ );
    }
}
 