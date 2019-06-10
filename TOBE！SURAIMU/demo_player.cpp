
// 基底クラス作成者 : 山田 晴久
// 板場 温樹

#include "demo_player.h"

#include "calc.h"
#include "Sound.h"
#include "space.h"

constexpr float kToRadian = XM_PI / 180.0F;
constexpr float kToDegree = 180.0F / XM_PI;


constexpr float kSpeed = 5.0F;              // プレイヤー速さ
constexpr float kSpeedRadian = kSpeed * kToRadian;
constexpr float kJumpTimming = 300.0F;      // ジャンプを入力する高さ
constexpr float kJumpAngleMin = -15.0F;
constexpr float kJumpAngleMax = 15.0F;
constexpr float kSquatAngleMin = -90.0F;
constexpr float kSquatAngleMax = 90.0F;
constexpr float kDeltaMin = -30.0F;
constexpr float kDeltaMax = 30.0F;
constexpr float kMaxChangeAngle = 70.0F * kToRadian;		//左右の最大変角度
constexpr float kRange = 200.0F;


/*===========================================================================*/
// init多重定義
bool DemoPlayer::init( const Vector2& Position )
{
    Player::init( Position , 0);

    is_jumping_ = false;
    serch_range_.set( myshape_.position, kRange );
    purpose_ = nullptr;


    return true;
}


/*===========================================================================*/
// インプットの上書き
void DemoPlayer::input( float Sin )
{

}

/*===========================================================================*/
// 近くにある検索対象オブジェクトを検索
void DemoPlayer::setPurpose()
{
    
}
