
// 板場　温樹

#include "ai_demo.h"

#include "release.h"
#include "calc.h"
#include "sercher.h"



/*===========================================================================*/
constexpr float kToRadians = XM_PI / 180.0F;
float toRadians( float Degrees ) { return Degrees * kToRadians; }

constexpr float kToDegrees = 180.0F / XM_PI;
float toDegrees( float Radians ) { return Radians * kToDegrees; }

constexpr float kWindowWidth  = getWindowWidth<float>();
constexpr float kWindowHeight = getWindowHeight<float>();

constexpr float kJumpRange = 400.0F;
constexpr float kJumpAngleErrorRange  = 4.0F;   // ジャンプする角度の誤差

constexpr float kSquatAngleErrorRange = 10.0F;  // しゃがむ角度の誤差


/*===========================================================================*/
AIDemo::AIDemo()
{

}

AIDemo::~AIDemo()
{
    destroy();
}


/*===========================================================================*/
bool AIDemo::init( const Vector2& Position, const int No )
{
    if( Player::init( Position, No ) == false ) { return false; }

    // 検索範囲の設定
    sercher_ = new Sercher();
    if( sercher_->init
        (
            {kWindowWidth / 2.0F, kWindowHeight / 2.0F},
            kWindowHeight / 2.0F,
            ObjectID::kStar
        ) == false )
    {
        return false;
    }

    // メンバ初期化
    purpose_ = nullptr;

    return true;
}

void AIDemo::destroy()
{
    if( sercher_ )
    {
    // 検索範囲オブジェクト開放
        sercher_->destroy();
        safe_delete( sercher_ );
    }

    Player::destroy();
}

void AIDemo::update()
{
    Player::update();   // 基底更新

    setPurpose();       // 目的の設定


    if( flag_.test(kJump) == false )
    {
    // 接地中
        if( direction_id_ != Direction::kSquat )
        {
        // しゃがみでない状態なら正面を向く
            direction_id_ = Direction::kFlont;
        }
    }
}


/*===========================================================================*/
bool AIDemo::isSquat()
{
    if( purpose_ )
    {
    // 目的がある
        float flont_angle = toDegrees(revision_angle_ + XM_PI);
        float jump_angle  = toDegrees(Calc::angle(purpose_->getPosition() - getPosition()));

        // 目的との角度が一定の範囲内ならtrue
        return std::abs(flont_angle - jump_angle) <= kSquatAngleErrorRange;
    }
    // 目的がない
    return false;
}

bool AIDemo::isJump()
{
    if( purpose_ == nullptr ) { return false; }


    bool jump = true;

    float flont_angle = toDegrees( revision_angle_ + XM_PI );
    float jump_angle = toDegrees(Calc::angle(purpose_->getPosition() - getPosition()));
    if( flont_angle < 0 ) { flont_angle += 360.0F; }
    if( jump_angle < 0 ) { jump_angle += 360.0F; }

    if( std::abs(flont_angle - jump_angle) > kJumpAngleErrorRange )
    {
    // 目的との角度が一定の範囲外
        jump = false;
    }
    // 目的との角度が一定の範囲内ならtrue
    return jump;
}


/*===========================================================================*/
void AIDemo::setPurpose()
{
    // 条件の設定
    bool (AIDemo::*check)( ObjectBase* const ) = nullptr;

    if( flag_.test(kJump) == false )
    {
        // 接地中
        check = &AIDemo::checkPurposeForGround;
    }
    else
    {
        float angle = toDegrees( revision_angle_ + XM_PI );
        if( angle < 0 ) { angle += 360.0F; }

        // 上昇中
        if( angle >= 0.0F && angle <= 180.0F )
        {
            check = &AIDemo::checkPurposeForUp;
        }
        // 下降中
        else
        {
            check = &AIDemo::checkPurposeForDown;
        }
    }


    // 目的の設定
    if( (this->*check)(purpose_) == false ) { purpose_ = nullptr; }
    for( auto star : sercher_->getList() )
    {
        if( (this->*check)(star) )
        {
            purpose_ = star;
        }
    }
}


/*===========================================================================*/
// 上昇中の条件
bool AIDemo::checkPurposeForUp( ObjectBase* const Target )
{
    if( Target == nullptr ) { return false; }


    // 現在のオーナーは目的としない
    if( Target == owner_ )  { return false; }


    return true;
}
// 下降中の条件
bool AIDemo::checkPurposeForDown( ObjectBase* const Target )
{
    using namespace Calc;

    if( Target == nullptr ) { return false; }


    // 上にあるスターは無視
    if( Target->getPosition().y < myshape_.position.y ) { return false; }


    // 現目的との比較
    if( purpose_ != nullptr )
    {
        float dist_pur = magnitude(purpose_->getPosition() - getPosition());
        float dist_tar = magnitude(Target->getPosition() - getPosition());

        // 近いほうを採用
        if( dist_tar > dist_pur ) { return false; }
    }

    return true;
}
// 接地中の条件
bool AIDemo::checkPurposeForGround( ObjectBase* const Target )
{
    if( Target == nullptr ) { return false; }


    // オーナーとの比較
    if( owner_ != nullptr )
    {
        // オーナーは目的としない
        if( Target == owner_ ) { return false; }

        // オーナー以下にあるスターは無視する
        if( Target->getPosition().y >= owner_->getPosition().y ) { return false; }
    }

    // 現目的との比較
    if( purpose_ != nullptr )
    {
        // 現目的よりも低いスターは無視
        if(Target->getPosition().y > purpose_->getPosition().y) {return false;}
    }

    // ジャンプで届く範囲外は無視
    if (Calc::magnitude(Target->getPosition() - getPosition()) > kJumpRange)
    {
        return false;
    }


    return true;
}
