
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

static constexpr int   kJumpPercentageDenominator = 2;
static constexpr float kJumpReach = 380.0F;
static constexpr float kJumpAngle = 10.0F;

static constexpr float kSquatAngle = 45.0F;

static constexpr float kJumpingMoveRange = 150.0F;


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
    setPurposeStar();


    last_position_ = getPosition();
    Player::update();
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
        // 目的との角度が一定の範囲内かつ上側か
        const Vector2& this_position = getPosition();
        const Vector2& tar_position  = target_->getPosition();
        float direction_angle = toDegrees( revision_angle_ + XM_PI );
        float between_angle = toDegrees(Calc::angle(tar_position - this_position) );
        float delta_angle = direction_angle - between_angle;

        if( this_position.x < tar_position.x ?
                              (delta_angle >= 0.0F && delta_angle <= kJumpAngle) :
                              (delta_angle <= 0.0F && delta_angle >= -kJumpAngle)
          )
        {
            // 一定の確率で
            if( !(rand() % kJumpPercentageDenominator) )
            {
                // ジャンプ!
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


// 目的関係
/*===========================================================================*/
// 目的となるスターを設定する
void AIDemo::setPurposeStar()
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
    
    // 条件に応じてターゲットを設定する
    if( !Player::isJump() )
    {
        // 接地中
        target_ = getTargetForStaying();
    }
    else
    {
        // 上昇中
        if( getPosition().y < last_position_.y )
        {
            target_ = getTargetForJumping();
        }
        // 下降中
        else
        {
            target_ = getTargetForFalling();
        }
    }
}
// ターゲットの取得( 接地中 )
// 範囲内にスターがある前提で処理を行う
Star* AIDemo::getTargetForStaying()
{
    // 条件式ラムダ
    auto judge = [this]( const ObjectBase* CurrTarget, const ObjectBase* TestTarget )->bool
    {
        const Vector2& this_position = getPosition();
        const Vector2& test_position = TestTarget->getPosition();

        // オーナーは不採用
        ObjectBase* const owner = this->getOwner();
        if( TestTarget == owner ) { return false; }

        // ジャンプの範囲外なら、不採用
        float dist_test = 
            std::pow(this_position.x - test_position.x, 2.0F) +
            std::pow(this_position.y - test_position.y, 2.0F);
        if( dist_test > std::pow( kJumpReach, 2.0F ) )  { return false; }

        // 現在の座標以下にあるものは、不採用
        if( (owner != nullptr) &&
            (test_position.y >= owner->getPosition().y)){ return false; }



        // 現在のターゲットが無かったら、採用
        if( CurrTarget == nullptr )                     { return true; }

        // 現在のターゲットと同じだったら、採用
        if( CurrTarget == TestTarget )                  { return true; }


        // 距離を比較 *距離のあるほうを採用( ジャンプの範囲内 )
        const Vector2& curr_position = CurrTarget->getPosition();
        float dist_curr = 
            std::pow(this_position.x - curr_position.x, 2.0F) + 
            std::pow(this_position.y - curr_position.y, 2.0F);
        return 
            dist_test > dist_curr &&
            dist_test <= std::pow( kJumpReach, 2.0F );
    };


    ObjectBase* target = target_;

    for( auto& star : sercher_->getList() )
    {
        if( judge( target, star ) )
        {
            target = star;
        }
    }

    return reinterpret_cast<Star*>(target);
}
// ターゲットの取得( ジャンプ(上昇中) )
Star* AIDemo::getTargetForJumping() const
{
    return target_;
}
// ターゲットの取得( 落下中 )
Star* AIDemo::getTargetForFalling() const
{
    return target_;
}
