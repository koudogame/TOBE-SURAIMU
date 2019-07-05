
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
constexpr float kSerchRange   = 300.0F;
constexpr float kDeltaJumpAngle = 2.5F;
constexpr float kMaxOffset = 3.0F;
constexpr float kSpeed = 0.1F;
constexpr float kJumpDisp = 450.0F;
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
    if( sercher_->init(
        {kWindowWidth / 2.0F, kWindowHeight / 2.0F},
        kWindowHeight,
        ObjectID::kStar) == false )
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

    if( !flag_.test( kJump ) &&
        jumping_ )
    {
        direction_id_ = Direction::kFlont;
        jumping_ = false;
        purpose_ = nullptr;
    }

    setPurpose();
    setJumpAngle();
}

/*===========================================================================*/
// 入力処理
void AIMover::inputjump()
{
    auto judge = [this]( float Angle )->bool
    {
        if( Angle < 0.0F ) { Angle += 360.0F; }

        // 画面外へトライ
        if( myshape_.position.y < 200.0F )
        {
            if( Angle >= 70.0F && Angle <= 110.0F &&
                (rand() % 5) == 0 )
            {
                return true;
            }
        }
        // 焦る高さじゃない
        else if( myshape_.position.y < 600.0F )
        {
            if( (std::abs(jump_angle_ - Angle) <= kDeltaJumpAngle) && // 目的との角度が誤差内
                ((myshape_.position.y >= 400.0F) || !(rand() % 2))    // 一定のライン以下か、n分の1の確立
              )
            {
                return true;
            }
        }
        // 画面外へ出てしまっている( 焦る )
        else
        {
            // 目的との角度が小さいとき
            if( std::abs( jump_angle_ - Angle) <= kDeltaJumpAngle )
            {
                return true;
            }
            else  if( Angle >= 85.0F && Angle <= 95.0F )
            {
                return true;
            }
        }

        return false;
    };


    float angle = TODEGREES( revision_angle_ + XM_PI );
    if( angle < 0.0F ) { angle += 360.0F; } 

    // ジャンプする角度との差が一定以内の値だった場合、しゃがむ
    if( std::abs(angle - jump_angle_) <= 20.0F )
    {
        direction_id_ = Direction::kSquat;
    }
    //ジャンプのタイミング
    if( judge(angle) &&
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
        if( (std::abs(purpose_->getPosition().x - myshape_.position.x) < 50.0F) &&
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
    // 消去法
        if( Target->isAlive() == false )
        {
            return false;
        }

        // 空中
        if( flag_.test(kJump) )
        {
            float angle = TODEGREES( std::atan2(-movement_.y, movement_.x) );

            // 上向き
            if( angle >= 0.0F && angle <= 180.0F )
            {
                // ターゲットが現在着地しているスターだった場合
                if( Target == owner_ )
                {
                    return false;
                }
            }
            // 下向き
            else
            {
                // ターゲットが自分よりも高い位置にあるスターか
                //  現在の目的よりも低い位置にあるか
                //   現在の目的よりも、横に遠い位置にある場合
                if( Target->getPosition().y < myshape_.position.y ||
                    (purpose_ != nullptr &&
                     (Target->getPosition().y > purpose_->getPosition().y ||
                     (std::abs(Target->getPosition().y - getPosition().y >
                      std::abs(purpose_->getPosition().x - getPosition().y)))
                     )
                    )
                  )
                {
                    return false;
                }
            }
        }
        // 接地
        else 
        {
            // ターゲットが、現在着地しているスターか、真横から下にあるスターだった場合
            if( owner_ != nullptr &&
                (Target == owner_ ||                           
                Target->getPosition().y >= owner_->getPosition().y)
              )
            {
                return false;
            }
        }

        if( purpose_ != nullptr )
        {
        // ジャンプで届かない範囲か、下にあるか
            const Vector2 kPurposePosition = purpose_->getPosition();
            const Vector2 kTargetPosition  = Target->getPosition();
            float disp_pur = magnitude( kPurposePosition - myshape_.position );
            float disp_tar = magnitude( kTargetPosition  - myshape_.position );
            if( disp_tar > kJumpDisp  ||
                kTargetPosition.y > kPurposePosition.y )
            {
                return false;
            }
        }

        return true;
    };

    if( purpose_ != nullptr && !check(purpose_) )
    {
        purpose_ = nullptr;
    }
    const auto kPurposes = sercher_->getList();
    const size_t kPurposesNum = kPurposes.size();
    for (size_t i = 0; i < kPurposesNum; ++i)
    {
        if( check( kPurposes[i]) )
        {
            purpose_ = kPurposes[i];
        }
    }
}
// ジャンプの角度を設定
void AIMover::setJumpAngle()
{
    if( purpose_ != nullptr )
    {
    // プレイヤーと、スターの原点をつないだ線分の角度をジャンプの角度とする
        Vector2 disp = purpose_->getPosition() - myshape_.position;
        jump_angle_ = TODEGREES( atan2(-disp.y, disp.x) );
        if( jump_angle_ < 0.0F ) { jump_angle_ += 360.0F; }
    }
    else
    {
        jump_angle_ = 500.0F;
    }
}
 