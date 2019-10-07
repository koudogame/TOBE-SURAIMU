
// 板場

#include "stage_select.h"

#include "textureLoder.h"
#include "sprite.h"
#include "pad.h"
#include "stage_name.h"

#include "title.h"

#include "stage_play.h"

// 定数
/*===========================================================================*/
static constexpr wchar_t kTextureFileName[] = L"Texture/stage_select.png";
static constexpr Vector2 kDrawBasePosition = { 384.0F, -1600.0F };
static constexpr unsigned kStageIDMin =  1U;
static constexpr unsigned kStageIDMax = 10U;
static constexpr Vector2 kOffsetUp   = { 0.0F, -204.8F };
static constexpr Vector2 kOffsetDown = { 0.0F,  204.8F };
static constexpr float kOffsetEasingConstant = 3.0F;
static constexpr float kCanSelectRestOffset = 5.0F;


// Ctor, Dtor
/*===========================================================================*/
StageSelect::StageSelect()
{

}

StageSelect::~StageSelect()
{

}


// override
/*===========================================================================*/
// 初期化関数
// 戻り値 [ true : 正常終了　false : エラー ]
bool StageSelect::init()
{
    // テクスチャの読み込み
    if( texture_ == nullptr )
    {
        texture_ = TextureLoder::getInstance()->load( kTextureFileName );
        if( texture_ == nullptr ) { return false; }
    }

    // メンバの初期化
    draw_position_ = kDrawBasePosition;
    selecting_stage_ = 1U;
    rest_offset_ = Vector2::Zero;

    return true;
}

/*===========================================================================*/
// 終了処理
void StageSelect::destroy()
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
// 遷移先は　タイトル　ステージプレイ
// 1.ステージを選択する
// 2.決定キーの入力で確定
SceneBase* StageSelect::update()
{
    const GamePad::State& kInputState = Pad::getInstance()->getState();

    offsetting();   // オフセットの移動

    // オフセットの移動が無かったらステージの選択を行う
    if( std::abs(rest_offset_.x) <= kCanSelectRestOffset &&
        std::abs(rest_offset_.y) <= kCanSelectRestOffset)
    {
        // ステージの選択
        if( selectStage( kInputState ) )
        {
            return new StagePlay( kStageFileName[selecting_stage_] );
        }
    }

    return this;
}

/*===========================================================================*/
// 描画処理
void StageSelect::draw()
{
    Sprite::getInstance()->reserveDraw(
        texture_,
        draw_position_
    );
}



/*===========================================================================*/
// ステージの選択
// 戻り値 [ true : ステージの決定　false : ステージの選択継続 ]
// 上入力で上方向にオフセット、ステージID減算
// 下入力で下方向にオフセット、ステージID加算
bool StageSelect::selectStage( const GamePad::State& InputState )
{
    // 選択中のステージを変更
    if( InputState.dpad.up &&
        selecting_stage_ > kStageIDMin )
    {
        --selecting_stage_;
        rest_offset_ += kOffsetUp;
    }
    else if( InputState.dpad.down &&
        selecting_stage_ < kStageIDMax )
    {
        ++selecting_stage_;
        rest_offset_ += kOffsetDown;
    }
    // 選択中のステージに決定
    else if( InputState.buttons.a )
    {
        return true;
    }

    return false;
}


/*===========================================================================*/
// オフセットの移動
void StageSelect::offsetting()
{
    Vector2 offset = rest_offset_ / kOffsetEasingConstant;
    rest_offset_ -= offset;
    draw_position_ += offset;

    if( std::abs(rest_offset_.x) <= 0.1F ) { rest_offset_.x = 0.0F; }
    if( std::abs(rest_offset_.y) <= 0.1F ) { rest_offset_.y = 0.0F; }
}
