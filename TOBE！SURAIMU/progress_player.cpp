
// 板場

#include "progress_player.h"

#include "textureLoder.h"
#include "sprite.h"

#include "player.h"


// 定数
/*===========================================================================*/
static constexpr wchar_t kTextureFile[] = L"Texture/progress.png";
static constexpr float kWidth = 70.0F;
static constexpr float kHeight = 70.0F;
static constexpr Vector2 kAnkerPoint { kWidth * 0.5F, kHeight * 0.5F };
static constexpr RECT kTrimming = { 140L, 0L, 210L, 70L };
static constexpr float kDrawDepth = 10.1F;

static constexpr float kScrollSpeed = 2.0F;


// Ctor, Dtor
/*===========================================================================*/

ProgressPlayer::ProgressPlayer()
{

}

ProgressPlayer::~ProgressPlayer()
{
    destroy();
}


/*===========================================================================*/
// 初期化処理
bool ProgressPlayer::init( Player* const Player,
                           const Vector2& Position,
                           const float Start, const float End,
                           const float Magnification )
{
    // テクスチャ読み込み
    if( texture_ == nullptr )
    {
        texture_ = TextureLoder::getInstance()->load( kTextureFile );
        if( texture_ == nullptr ) { return false; }     // エラー
    }


    // 引数の値をメンバにセット
    player_        = Player;
    position_      = Position;
    start_line_    = Start;
    end_line_      = End;
    magnification_ = Magnification;

    // その他メンバの初期化
    last_position_y_ = player_->getPosition().x - 720.0F;

    return true;
}

/*===========================================================================*/
// 終了処理
void ProgressPlayer::destroy()
{
    // テクスチャ開放
    if( texture_ != nullptr )
    {
        TextureLoder::getInstance()->release( texture_ );
        texture_ = nullptr;
    }
}

/*===========================================================================*/
// 更新処理
void ProgressPlayer::update()
{
    // 移動
    position_.y += getMovementDelta();
}

/*===========================================================================*/
// 描画処理
void ProgressPlayer::draw( const Vector2& Offset )
{
    Sprite::getInstance()->reserveDraw(
        texture_,
        position_ + Offset,
        kTrimming,
        1.0F,           // alpha
        kDrawDepth,
        { 1.0F, 1.0F }, // scale
        0.0F,           // rotate
        kAnkerPoint
    );







    /*Sprite::getInstance()->reserveDraw(
        texture_,
        { position_.x, start_line_ },
        kTrimming,
        1.0F,
        kDrawDepth-0.1,
        {1.0F, 1.0F},
        0.0F,
        kAnkerPoint
    );
    Sprite::getInstance()->reserveDraw(
        texture_,
        { position_.x, end_line_ },
        kTrimming,
        1.0F,
        kDrawDepth-0.1,
        { 1.0F, 1.0F },
        0.0F,
        kAnkerPoint
    );*/
}


/*===========================================================================*/
// スクロール
void ProgressPlayer::setMove( const float Dist )
{
    position_.y += (-Dist - kScrollSpeed) * magnification_;
}


// private
/*===========================================================================*/
// 描画位置の移動量を取得
float ProgressPlayer::getMovementDelta()
{
    float curr_position_y = player_->getPosition().y;
    float disp = curr_position_y - last_position_y_;


    last_position_y_ = curr_position_y;

    return disp * magnification_;
}
