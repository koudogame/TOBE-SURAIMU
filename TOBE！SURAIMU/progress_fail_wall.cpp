
// 板場

#include "progress_fail_wall.h"

#include "textureLoder.h"
#include "sprite.h"

#include "player.h"
#include "fail_wall.h"
#include "progress_player.h"



// 定数
/*===========================================================================*/
static constexpr wchar_t kTextureFile[] = L"Texture/progress.png";
static constexpr RECT    kTrimming = { 70, 0L, 140L, 720L };
static constexpr float   kDrawDepth = 10.2F;
static constexpr Vector2 kDrawAdjust { 0.0F, -50.0F };


// Ctor, Dtor
/*===========================================================================*/
ProgressFailWall::ProgressFailWall()
{

}

ProgressFailWall::~ProgressFailWall()
{
    destroy();
}


/*===========================================================================*/
// 初期化処理
bool ProgressFailWall::init( FailWall* const FailWall,
                             Player* const Player,
                             ProgressPlayer* const ProgressPlayer,
                             const Vector2& Position,
                             const float Magnification )
{
    // テクスチャ読み込み
    if( texture_ == nullptr )
    {
        texture_ = TextureLoder::getInstance()->load( kTextureFile );
        if( texture_ == nullptr ) { return false; }     // エラー
    }

    // 引数の値をメンバにセット
    fail_wall_       = FailWall;
    player_          = Player;
    progress_player_ = ProgressPlayer;
    position_        = Position;
    magnification_   = Magnification;

    return true;
}

/*===========================================================================*/
// 終了処理
void ProgressFailWall::destroy()
{
    // テクスチャ開放
    if( texture_ != nullptr )
    {
        TextureLoder::getInstance()->release( texture_ );
    }
}

/*===========================================================================*/
// 更新処理
void ProgressFailWall::update()
{
    position_.y = getDistanceProgressPlayer();
}

/*===========================================================================*/
// 描画処理
void ProgressFailWall::draw()
{
    Sprite::getInstance()->reserveDraw(
        texture_,
        position_ + kDrawAdjust,
        kTrimming,
        1.0F, // alpha
        kDrawDepth
    );
}


/*===========================================================================*/
// プレイヤーとの、進行度上の距離を取得する
float ProgressFailWall::getDistanceProgressPlayer() const
{
    // 等倍での、炎とプレイヤーの距離
    float distance = fail_wall_->getPosition().y - player_->getPosition().y;

    // 進行度上での、距離を返却
    return progress_player_->getPosition().y + (distance * magnification_);
}
