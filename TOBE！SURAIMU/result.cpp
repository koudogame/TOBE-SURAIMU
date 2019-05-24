
#include "result.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"

#include "text.h"
#include "scoring.h"
#include "ranking_manager.h"
#include "title.h"
#include "endless.h"


using KeyState = Keyboard::State;
using KeyTracker = Keyboard::KeyboardStateTracker;
using PadState = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;


bool isMove(unsigned * const Counter);

/*===========================================================================*/
constexpr unsigned kFrameWait = 10U;

/*===========================================================================*/
Result::Result(const unsigned Rank, const Scoring& Score) :
    score_(Score),
    rank_(Rank)
{
}

Result::~Result()
{
    destroy();
}


/*===========================================================================*/
// 初期化処理
bool Result::init()
{
    if( created_ == true ) { return true; }
    created_ = true;

    // テクスチャ読み込み
	texture_ = TextureLoder::getInstance()->load(L"Texture/リザルト.png");
    if (texture_ == nullptr) { return false; }

    // メンバ
    for( auto& name : name_ )
    {
        name = kCharTable[36]; // ' 'で初期化
    }
    name_[kNameMax] = '\0';    // 終端文字
    index_name_ = 0U;
    index_char_ = 10;
    count_frame_ = 0U;
    update_ = rank_ <= kRegisteredNum ? &Result::setName : &Result::selectNext;
    position_.x = 0.0F;
    position_.y = 0.0F;

	return true;
}

/*===========================================================================*/
// 終了処理
void Result::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;

    // テクスチャ開放
	TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
SceneBase* Result::update()
{
    return (this->*update_)();
}

/*===========================================================================*/
// 描画処理
void Result::draw()
{
    // 背景
	Sprite::getInstance()->draw( texture_, Vector2::Zero );
}

/*===========================================================================*/
// ランクイン時、名前の設定
SceneBase* Result::setName()
{
    KeyState   key_state   = Key::getInstance()->getState();
    KeyTracker key_tracker = Key::getInstance()->getTracker();
    PadState   pad_state   = Pad::getInstance()->getState();
    PadTracker pad_tracker = Pad::getInstance()->getTracker();

    // 決定
    if( key_tracker.pressed.Enter || pad_tracker.b == PadTracker::PRESSED )
    {
        name_[index_name_] = kCharTable[index_char_];
        ++index_name_;
        if( index_name_ > kNameMax )
        {
            --index_name_;
            update_ = &Result::selectNext;
        }
    }
    // 選択
    else if( 
        // 新規入力か
        ( (key_tracker.pressed.Up ||
            pad_tracker.leftStickUp == PadTracker::PRESSED) ) ||
        // 一定時間の長押しで
        ( (key_state.Up || pad_state.IsLeftThumbStickUp()) && 
            isMove(&count_frame_)) )
    {
        ++index_char_;
    }
    else if( 
        // 新規入力か
        ( (key_tracker.pressed.Down ||
            pad_tracker.leftStickDown == PadTracker::PRESSED) ) ||
        // 一定時間の長押しで
        ( (key_state.Down || pad_state.IsLeftThumbStickDown()) &&
            isMove(&count_frame_)) )
    {
        --index_char_;
    }

    // 循環させる
    if(index_char_ < 0 ) { index_char_ = kCharNum - 1; }
    if(index_char_ > kCharNum ) { index_char_ = 0; }

    return this;
}

/*===========================================================================*/
// 次のモードを選択
SceneBase* Result::selectNext()
{
    return this;
}


/*===========================================================================*/
// 長押し時、変化のタイミングかどうかを返す
bool isMove( unsigned * const Counter )
{
    ++(*Counter);
    if( *Counter >= kFrameWait )
    {
        *Counter = 0U;
    }
    return (*Counter) == 0U;
}