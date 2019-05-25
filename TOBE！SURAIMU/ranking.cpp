
// 板場 温樹

#include "ranking.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"

#include "title.h"


using KeyState = Keyboard::State;
using KeyTracker = Keyboard::KeyboardStateTracker;
using PadState = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;

void addMagnification(float* const Val);
void addOffset(float* const Val, const float AddVal);

/*===========================================================================*/
constexpr float kLineHeight = 25.0F;
constexpr float kMagnification = 0.1F;
constexpr float kMagnificationMax = 2.0F;
constexpr float kOffset = 5.0F;
constexpr float kOffsetMax = 100.0F;
const RECT kTrimmingBackground{
    0L, 0L, getWindowWidth<long>(), getWindowHeight<long>() };


/*===========================================================================*/
Ranking::Ranking()
{
}

Ranking::~Ranking()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool Ranking::init()
{
    if( created_ == true ) { return true; }
    created_ = true;


    // テクスチャ読み込み
    texture_ = TextureLoder::getInstance()->load(L"Texture/title.png");
    if( texture_ == nullptr ) { return false; }


    // メンバ初期化
    magnification_ = 1.0F;
    offset_ = 0.0F;

    return true;
}

/*===========================================================================*/
// 終了処理
void Ranking::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;

    // テクスチャ開放
    TextureLoder::getInstance()->release( texture_ );
}

/*===========================================================================*/
// 更新処理
SceneBase* Ranking::update()
{
    KeyState   key_state = Key::getInstance()->getState();
    KeyTracker key_tracker = Key::getInstance()->getTracker();
    PadState   pad_state = Pad::getInstance()->getState();
    PadTracker pad_tracker = Pad::getInstance()->getTracker();

    // 上新規入力時、1列分上へスクロール
    if( key_tracker.pressed.Up ||
        pad_tracker.leftStickUp == PadTracker::PRESSED )
    {
        magnification_ = 1.0F;
        addOffset( &offset_, -kLineHeight );
    }
    // 下新規入力時、1列分下へスクロール
    else if( key_tracker.pressed.Down ||
        pad_tracker.leftStickDown == PadTracker::PRESSED )
    {
        magnification_ = 1.0F;
        addOffset( &offset_, kLineHeight );
    }
    // 上長押しでスクロール( 押している間スクロールスクロール倍率を上げる )
    else if( key_state.Up || pad_state.IsLeftThumbStickUp() )
    {
        addOffset( &offset_, -kOffset * magnification_ );
        addMagnification( &magnification_ );
    }
    // 下長押しでスクロール( 押している間スクロールスクロール倍率を上げる )
    else if( key_state.Down || pad_state.IsLeftThumbStickDown() )
    {
        addOffset( &offset_, kOffset * magnification_ ) ;
        addMagnification( &magnification_ );
    }
    // Enterかaボタンが離されたら決定
    else if(key_tracker.released.Enter || pad_tracker.a ==PadTracker::RELEASED)
    {
        return new Title();
    }


    return this;
}

/*===========================================================================*/
// 描画処理
void Ranking::draw()
{
    // 背景の描画
    Sprite::getInstance()->draw(texture_, Vector2::Zero, &kTrimmingBackground);

    // ランキングの描画
}


/*===========================================================================*/
// 倍率の加算( 上限を超えないよう定数を加算 )
void addMagnification( float* const Val )
{
    *Val += kMagnification;
    if( *Val > kMagnificationMax ) { *Val = kMagnificationMax; }
}
// オフセット量の加算( 上限を超えない、下限を下回らないよう加算 )
void addOffset( float* const Val, const float Add )
{
    *Val += Add;

    if( *Val > kOffsetMax ) { *Val = kOffsetMax; }
    else if( *Val < 0.0F )  { *Val = 0.0F; }
}
