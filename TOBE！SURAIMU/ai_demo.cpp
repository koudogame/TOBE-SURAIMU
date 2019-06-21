
// 基底クラス作成者 : 山田 晴久
// 板場 温樹

#include "ai_demo.h"
#include "sercher.h"

#include "calc.h"
#include "Sound.h"
#include "space.h"

/*===========================================================================*/
constexpr float kToRadian = XM_PI / 180.0F;
constexpr float kToDegree = 180.0F / XM_PI;

constexpr float kJumpHeight = 500.F;
constexpr float kJumpAngleMin = -15.0F;
constexpr float kJumpAngleMax = 15.0F;
constexpr float kSerchRangeRadius = kJumpHeight / 2.0F;

/*===========================================================================*/
AIDemo::AIDemo()
{

}

AIDemo::~AIDemo()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool AIDemo::init( const Vector2& Position, const int PlayerNo )
{
    Player::init( Position , PlayerNo);

    sercher_ = new Sercher();
    if( sercher_->init( 
        Vector2{ Position.x, Position.y - kSerchRangeRadius },
        kSerchRangeRadius,
        ObjectID::kStar) == false )
    {
        return false;
    }

    purpose_ = nullptr;


    return true;
}


/*===========================================================================*/
// 終了処理
void AIDemo::destroy()
{
    Player::destroy();

    if( sercher_ ) 
    {
        sercher_->destroy();
        safe_delete( sercher_ ); 
    }
}


/*===========================================================================*/
// 更新処理
void AIDemo::update()
{
    // スター検索範囲を正面に設定
    float angle = base_angle_ + XM_PI;
    sercher_->setOrigin( {
        myshape_.position.x + std::cos(angle) * kSerchRangeRadius,
        myshape_.position.y - std::sin(angle) * kSerchRangeRadius
        } );

    Player::update();
}


/*===========================================================================*/
// 入力処理
void AIDemo::inputjump()
{
    direction_id_ = Direction::kFlont;
    purpose_ = nullptr;

    // 目標を設定
    const std::deque<ObjectBase*>& kTargets = sercher_->getList();
    const size_t kTargetsNum = kTargets.size();
    if( purpose_ == nullptr &&
        kTargetsNum > 0 )
    {
        for( size_t i = 0U; i < kTargetsNum && purpose_ == nullptr; ++i )
        {
            purpose_ = kTargets[ rand() % kTargetsNum ];
            if( (purpose_ == owner_) ||
                (purpose_->getPosition().y > getPosition().y) ) 
            {
                purpose_ = nullptr;
            }
        }
    }

    if( purpose_ != nullptr && !died_flag_ ) {
        //ジャンプ
        float angle = Calc::angle( purpose_->getPosition() - getPosition() );
        angle -= revision_angle_ + XM_PI;
        angle *= kToDegree;
        if( angle >= kJumpAngleMin && angle <= kJumpAngleMax )
        {
            // ジャンプ
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
            prev_jump_moveamount_ = 0;
            particle_alpha_ = 1.0F;
            score_.resetRotate();
        }
    }
}

void AIDemo::inputmove()
{}

