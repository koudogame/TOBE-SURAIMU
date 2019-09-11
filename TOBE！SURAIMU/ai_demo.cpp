
// 板場

#include "ai_demo.h"

#include "release.h"
#include "shape.h"
#include "calc.h"
#include "sercher.h"
#include "star.h"



/*===========================================================================*/
static constexpr float kToDegrees = 180.0F / XM_PI;
constexpr float toDegrees( const float Radians ) { return Radians * kToDegrees; }
static constexpr float kToRadians = XM_PI / 180.0F;
constexpr float toRadians( const float Degrees ) { return Degrees * kToRadians; }

// 定数
/*===========================================================================*/
static const Circle kSerchRange 
{
    { getWindowWidth<float>() * 0.5F, getWindowHeight<float>() * 0.5F },
    getWindowHeight<float>() * 0.5F
};

static constexpr int   kJumpPercentageDenominator = 1;
static constexpr float kJumpReach = 380.0F;
static constexpr float kJumpAngleMin = 5.0F;
static constexpr float kJumpAngleMax = 15.0F;

static constexpr float kSquatAngle = 45.0F;

static constexpr float kJumpingMoveRange = 200.0F;


// Ctor, Dtor
/*===========================================================================*/
AIDemo::AIDemo()
{

}
AIDemo::~AIDemo()
{

}


// override( public )
/*===========================================================================*/
// 初期化処理
bool AIDemo::init( const Vector2& Position, const int No )
{
    if( Player::init( Position, No ) == false ) { return false; }


    // 検索範囲初期化
    if( sercher_ == nullptr ) { sercher_ = new Sercher(); }
    if( sercher_->init(kSerchRange.position, kSerchRange.radius, ObjectID::kStar) == false )
    {
        return false;
    }


    // その他のメンバ初期化
    target_      = nullptr;
    last_position_ = getPosition();
    is_jumping_ = false;


    return true;
}
// 終了処理
void AIDemo::destroy()
{
    if( sercher_ != nullptr )
    {
        sercher_->destroy();
        safe_delete( sercher_ );
    }
   

    Player::destroy();
}
// 更新処理
void AIDemo::update()
{
    setTarget();


    last_position_ = getPosition();
    Player::update();

    if( Player::isJump() == false && is_jumping_ == true )
    {
        is_jumping_ = false;
        target_ = nullptr;
    }
}


// override( private )
/*===========================================================================*/
// しゃがむか判定
bool AIDemo::isSquat()
{
    // ターゲットがあるか
    if( target_ != nullptr )
    {
        // ターゲットとの角度が一定の範囲内か
        const Vector2& this_position = getPosition();
        const Vector2& tar_position  = target_->getPosition();
        float direction_angle = toDegrees( revision_angle_ + XM_PI );
        float between_angle = toDegrees( Calc::angle(tar_position - this_position) );

        if( std::abs(direction_angle - between_angle) <= kSquatAngle )
        {
            // しゃがむ!
            return true;
        }
    }

    return false;
}
// ジャンプするか判定
bool AIDemo::isJump()
{
    // ターゲットがあるか
    if( target_ != nullptr )
    {
        // 目的との角度が一定の範囲内
        const Vector2& this_position = getPosition();
        const Vector2& tar_position  = target_->getPosition();
        float direction_angle = toDegrees( revision_angle_ + XM_PI );
        float between_angle = toDegrees(Calc::angle(tar_position - this_position) );
        float delta_angle = direction_angle - between_angle;

        if( this_position.x < tar_position.x ?
                              (delta_angle >= kJumpAngleMin && delta_angle <= kJumpAngleMax) :
                              (delta_angle <= -kJumpAngleMin && delta_angle >= -kJumpAngleMax)
          )
        {
            // 一定の確率で
            if( !(rand() % kJumpPercentageDenominator) )
            {
                // ジャンプ!
                is_jumping_ = true;
                return true;
            }
        }
    }


    return false;
}
// 左に移動するか判定
bool AIDemo::isMoveLeft()
{
    // ジャンプ中なら
   if( Player::isJump() )
   {
       // ターゲットがあれば
       if( target_ != nullptr )
       {
           const Vector2& tar_posi = target_->getPosition();
           const Vector2& this_posi= this->getPosition();
           // ターゲットが自分よりも左に位置していたら
           if( tar_posi.x + target_->getSize() < this_posi.x )
           {
               // ターゲットとの距離が移動可能距離内なら
               if( Calc::magnitude( tar_posi, this_posi ) <= kJumpingMoveRange )
               {
                   return true;
               }
           }
       }
   }

    return false;
}
// 右に移動するか判定
bool AIDemo::isMoveRight()
{
    // ジャンプ中なら
    if( Player::isJump() )
    {
        // ターゲットがあるか
        if( target_ != nullptr )
        {
            const Vector2& tar_posi = target_->getPosition();
            const Vector2& this_posi= this->getPosition();
            // ターゲットが自分よりも右に位置していたら
            if( tar_posi.x - target_->getSize() > this_posi.x )
            {
                // ターゲットとの距離が移動可能範囲内なら
                if( Calc::magnitude( tar_posi, this_posi ) <= kJumpingMoveRange )
                {
                    return true;
                }
            }
        }
    }

    return false;
}
// 下に移動するか判定
bool AIDemo::isMoveDown()
{
    return false;
}


// ターゲッティング関係
/*===========================================================================*/
// ターゲットとなるスターを設定する
void AIDemo::setTarget()
{
    // 現在のターゲットが、炎と衝突していたらリセット
    if( target_ != nullptr && target_->isAlive() == false )
    {
        target_ = nullptr;
    }

    // 範囲内にスターが無かったら終了
    if( sercher_->getList().size() <= 0 ) 
    {
        target_ = nullptr;
        return; 
    }
    

    auto judge = getJudgeFunc();
    for( auto& star : sercher_->getList() )
    {
        if( (this->*judge)( star ) )
        {
            target_ = reinterpret_cast<Star*>(star);
        }
    }
}
// ターゲット設定の条件式を取
bool(AIDemo::*AIDemo::getJudgeFunc())(ObjectBase* const)
{
    // 条件に応じてターゲットを設定する
    if( !Player::isJump() )
    {
        // 接地中
        return &AIDemo::judgeForStaying;
    }
    else
    {
        // 上昇中
        if( getPosition().y < last_position_.y )
        {
            return &AIDemo::judgeForJumping;
        }
        // 下降中
        else
        {
            return &AIDemo::judgeForFalling;
        }
    }
}
// 着地中のターゲット条件式
bool AIDemo::judgeForStaying( ObjectBase* const Target )
{
#if true
    const Vector2& this_posi = getPosition();
    const Vector2& targ_posi = Target->getPosition();

    // オーナーは不採用
    if( owner_ != nullptr )
    {
        if( Target == this->getOwner() ) { return false; }

        // オーナーより下にあるものは不採用
        if( targ_posi.y > owner_->getPosition().y ) { return false; }
    }


    auto getDist2 = [this]( const Vector2& U, const Vector2& V )->float
    {
        return std::pow(U.x - V.x, 2.0F) + std::pow(U.y - V.y, 2.0F);
    };

    // ジャンプで届く範囲外にあるものは不採用
    float dist_new_targ = getDist2(this_posi, targ_posi);
    if( dist_new_targ > std::pow(kJumpReach, 2.0F) ) { return false; }

    if( target_ != nullptr )
    {
        // 現在のターゲットとの距離よりも遠かったら不採用
        float dist_now_targ = getDist2(this_posi, target_->getPosition());
        if( dist_new_targ > dist_now_targ ) { return false; }
    }



   
    return true;

#else
    const Vector2& this_position = getPosition();
    const Vector2& test_position = Check->getPosition();

    // オーナーは不採用
    ObjectBase* const owner = this->getOwner();
    if( Check == owner ) { return false; }

    // ジャンプの範囲外なら、不採用
    float dist_test = 
        std::pow(this_position.x - test_position.x, 2.0F) +
        std::pow(this_position.y - test_position.y, 2.0F);
    if( dist_test > std::pow( kJumpReach, 2.0F ) )  { return false; }

    // 現在の座標以下にあるものは、不採用
    if( (owner != nullptr) &&
        (test_position.y >= owner->getPosition().y) ){ return false; }



    // 現在のターゲットが無かったら、採用
    if( target_ == nullptr )                        { return true; }

    // 現在のターゲットと同じだったら、採用
    if( target_ == Check )                          { return true; }


    // 距離を比較 *距離のあるほうを採用
    const Vector2& curr_position = target_->getPosition();
    float dist_curr = 
        std::pow(this_position.x - curr_position.x, 2.0F) + 
        std::pow(this_position.y - curr_position.y, 2.0F);
    return dist_test > dist_curr;
#endif
}
// 上昇中のターゲット条件式
bool AIDemo::judgeForJumping( ObjectBase* const Check )
{
    return false;
}
// ターゲットの取得( 落下中 )
bool AIDemo::judgeForFalling( ObjectBase* const Check )
{
    // 自分よりも上にある上にあるものは不採用
    if( Check->getPosition().y < getPosition().y ) { return false; }

    // 現在のターゲットと比較して、より遠かったら不採用
    if( target_ != nullptr &&
        (Calc::magnitude( Check->getPosition(), getPosition() ) >
         Calc::magnitude( target_->getPosition(), getPosition() )) )
    {
        return false;
    }
     

    return true;
}
