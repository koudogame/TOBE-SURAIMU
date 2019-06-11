
// 基底クラス作成者 : 山田 晴久
// 板場 温樹

#include "demo_player.h"
#include "sercher.h"

#include "calc.h"
#include "Sound.h"
#include "space.h"

/*===========================================================================*/
constexpr float kToRadian = XM_PI / 180.0F;
constexpr float kToDegree = 180.0F / XM_PI;

constexpr float kJumpHeight = 500.F;
constexpr float kSerchRangeRadius = kJumpHeight / 2.0F;

/*===========================================================================*/
DemoPlayer::DemoPlayer()
{

}

DemoPlayer::~DemoPlayer()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool DemoPlayer::init( const Vector2& Position, const int PlayerNo )
{
    Player::init( Position , PlayerNo);

    is_jumping_ = false;
    sercher_ = new Sercher( 
        Vector2{ Position.x, Position.y - kSerchRangeRadius },
        kSerchRangeRadius,
        ObjectID::kStar);
    purpose_ = nullptr;


    return true;
}


/*===========================================================================*/
// 終了処理
void DemoPlayer::destroy()
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
void DemoPlayer::update()
{
    // スター検索範囲を正面に設定
    float angle = revition_angle_ + XM_PI;
    sercher_->setOrigin( {
        std::cos(angle)  * kSerchRangeRadius,
        std::sin(-angle) * kSerchRangeRadius
        } );

    Player::update();
}


/*===========================================================================*/
// 入力処理
void DemoPlayer::input( float Sin )
{
    // TODO : ここで目的のスターを選択する
    const std::deque<ObjectBase*>& obj_list = sercher_->getList();

    if( flag_.test(kJump) )
    {

    }
    else
    {
        direction_id_ = Direction::kFlont;
        movement_angle_ = revition_angle_;


        if( purpose_ == nullptr &&
            obj_list.size() > 0 )
        {
            purpose_ = obj_list[ rand() % obj_list.size() ];
        }

        if( purpose_ != nullptr )
        {
            //ジャンプ
            if (!died_flag_)
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
            }
        }
    }
}

