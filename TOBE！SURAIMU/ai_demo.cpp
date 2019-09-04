
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
static constexpr float kJumpAngle = 5.0F;

static constexpr float kSquatAngle = 45.0F;


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
    if( sercher_ == nullptr )
    {
        sercher_ = new Sercher();
    }

    if( sercher_->init(kSerchRange.position, kSerchRange.radius, ObjectID::kStar) == false )
    {
        return false;
    }


    // その他のメンバ初期化
    target_      = nullptr;


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
        // 目的との角度が一定の範囲内か
        const Vector2& this_position = getPosition();
        const Vector2& tar_position  = target_->getPosition();
        float direction_angle = toDegrees( revision_angle_ + XM_PI );
        float between_angle = toDegrees(Calc::angle(tar_position - this_position) );

        if( std::abs(direction_angle - between_angle) <= kJumpAngle )
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
    // ジャンプ中かつなら
    if( isJump() )
    {

    }

    return false;
}
// 右に移動するか判定
bool AIDemo::isMoveRight()
{
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
    // 範囲内にスターが無かったら終了
    if( sercher_->getList().size() <= 0 ) 
    {
        target_ = nullptr;
        return; 
    }

    // 現在のターゲットが、炎と衝突していたらリセット
    if( target_ != nullptr && target_->isAlive() == false )
    {
        target_ = nullptr;
    }

    
    // 条件に応じて目的のスターを設定する
    Star* target = nullptr;
    if( !isJump() )
    {
        // 接地
        target = getTargetForStaying();
    }
    else
    {
        // 空中
    }


    // ターゲットを設定
    target_ = target;
}
// ターゲットの取得( 接地中 )
// 範囲内にスターがある前提で処理を行う
//
// 範囲内のスターで、最も距離のあるものをターゲットとする
// 現在の座標より下にあるものは、ターゲットとしない
Star* AIDemo::getTargetForStaying()
{
    // 条件式ラムダ
    auto judge = [this]( const ObjectBase* CurrTarget, const ObjectBase* TestTarget )->bool
    {
        const Vector2& this_position = getPosition();
        const Vector2& test_position = TestTarget->getPosition();

        // ジャンプの範囲外なら、不採用
        float dist_test = 
            std::pow(this_position.x - test_position.x, 2.0F) +
            std::pow(this_position.y - test_position.y, 2.0F);
        if( dist_test > std::pow( kJumpReach, 2.0F ) )  { return false; }

        // 現在の座標以下にあるものは、不採用
        ObjectBase* const owner = this->getOwner();
        if( owner != nullptr &&
            test_position.y >= owner->getPosition().y ) { return false; }



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
