
// 板場 温樹

#include "pause.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"

#include "Sound.h"


using KeyTracker = Keyboard::KeyboardStateTracker;
using PadTracker = GamePad::ButtonStateTracker;


/*===========================================================================*/
constexpr float kWindowWidth     = getWindowWidth<float>();
constexpr float kWindowHeight    = getWindowHeight<float>();
constexpr float kWindowWidthHarf = kWindowWidth / 2.0F;

constexpr long kWaitCount    = 3L;
constexpr long kCountTimeMs  = 1000L;
constexpr long kWaitTimeMs = kCountTimeMs * kWaitCount;

constexpr long kWidth        = 270L;
constexpr long kHeight       = 270L;

constexpr long kCursorWidth  = 270L;
constexpr long kCursorHeight = 16L;
constexpr float kCursorMove  = 48.0F;

constexpr long kNumberWidth = 126L;
constexpr float kNumberScaleMin      = 1.0F;
constexpr float kNumberScaleMax      = 1.3F;
constexpr float kNumberScaleDefault  = kNumberScaleMin;
constexpr float kNumberScaleAddPerMs = (kNumberScaleMax - kNumberScaleMin) / kCountTimeMs;
constexpr float kNumberAlphaMin      = 0.0F;
constexpr float kNumberAlphaMax      = 1.0F;
constexpr float kNumberAlphaDefault  = kNumberAlphaMax;
constexpr float kNumberAlphaAddPerMs = (kNumberAlphaMin - kNumberAlphaMax) / kCountTimeMs / 4;

constexpr Vector2 kPosition( kWindowWidthHarf - (kWidth  / 2.0F),
                         (kWindowHeight / 2.0F) - (kHeight / 2.0F) );
constexpr Vector2 kPositionCursorStart( kWindowWidthHarf - (kCursorWidth / 2.0F),
                                    345.0F );
constexpr Vector2 kPositionRemainingTime { 640.0F, 360.0F };
constexpr Vector2 kAnkerRemainingTime { 63.0F, 85.0F };

enum { kPause, kBack, kCursor, kNumber };
const RECT kTrimming[] = {
    {   0L,   0L,  270L, 270L },
    { 270L,   0L, 1550L, 720L },
    {   0L, 270L,  269L, 289L },
    {   0L, 720L,  126L, 890L },
};

/*===========================================================================*/
Pause::Pause()
{
}

Pause::~Pause()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool Pause::init()
{
    destroy();
    created_ = true;

    texture_      = TextureLoder::getInstance()->load(L"Texture/pause.png");
    if( texture_ == nullptr )      { return false; }

    reset();


    return true;
}

/*===========================================================================*/
// 終了処理
void Pause::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;

    if( texture_ )      { TextureLoder::getInstance()->release(texture_); }
}

/*===========================================================================*/
// 更新処理
int Pause::update()
{
    return (this->*update_)();
}

/*===========================================================================*/
// 描画処理
void Pause::draw() const
{


    Sprite* kSprite = Sprite::getInstance();

    // 入力待機時のみ描画
    if( update_ == &Pause::waitInput )
    {
        // 背景
	    kSprite->reserveDraw( texture_ , Vector2::Zero ,  &kTrimming[kBack] , 1.0F , 1.0F );
        // ポーズ本体
        kSprite->reserveDraw( texture_, kPosition,        &kTrimming[kPause], 1.0F, 1.0F );

        // カーソル
        kSprite->reserveDraw( texture_, position_cursor_, &kTrimming[kCursor], 1.0F, 1.0F );
    }
    // コンティニュー時にのみ描画
    else if( update_ == &Pause::toContinue )
    {
        RECT trimming = kTrimming[kNumber];
        trimming.left += (kWaitCount - remaining_count_) * kNumberWidth;
        trimming.right = trimming.left + kNumberWidth;

        // 残り時間
        kSprite->reserveDraw( texture_, kPositionRemainingTime, &trimming,
            number_alpha_, 0.0F, {number_scale_, number_scale_}, 0.0F,
            kAnkerRemainingTime );
    }

}

/*===========================================================================*/
// メンバリセット
void Pause::reset()
{
    update_ = &Pause::waitInput;
    select_ = kContinue;
    count_started_ = false;
    position_cursor_ = kPositionCursorStart;
    remaining_count_ = 0L;
    number_scale_ = kNumberScaleDefault;
    number_alpha_ = kNumberAlphaDefault;
}

// 更新関数小分け
/*===========================================================================*/
// 入力待ち
int Pause::waitInput()
{
    KeyTracker key = Key::getInstance()->getTracker();
    PadTracker pad = Pad::getInstance()->getTracker();

    // 決定
    if( pad.a == PadTracker::RELEASED || key.released.Space )
    {
        SOUND->stop(SoundId::kDicision);
        SOUND->play(SoundId::kDicision, false);

        switch (select_)
        {
        case kContinue:
            update_ = &Pause::toContinue; break;
        case kRestart:
            update_ = &Pause::toRestart;  break;
        case kTitle:
            update_ = &Pause::toTitle;    break;
        }
    }
    // 再度ポーズボタンの押下
    else if( pad.menu == PadTracker::PRESSED ||
             key.pressed.P )
    {
        update_ = &Pause::toContinue;
    }
    // 上選択
    else if (pad.leftStickUp == PadTracker::PRESSED ||
             pad.dpadUp      == PadTracker::PRESSED ||
             key.pressed.Up)
    {
        if (select_ > kContinue)
        {
            --select_;
            position_cursor_.y -= kCursorMove;
            SOUND->stop(SoundId::kSelect);
            SOUND->setPitch(SoundId::kSelect, 0.0F);
            SOUND->play(SoundId::kSelect, false);
        }
        else
        {
            SOUND->stop(SoundId::kSelect);
            SOUND->setPitch(SoundId::kSelect, -0.5F);
            SOUND->play(SoundId::kSelect, false);
        }
    }
    // 下選択
    else if (pad.leftStickDown == PadTracker::PRESSED ||
             pad.dpadDown      == PadTracker::PRESSED ||
             key.pressed.Down)
    {
        if (select_ < kTitle)
        {
            ++select_;
            position_cursor_.y += kCursorMove;
            SOUND->stop(SoundId::kSelect);
            SOUND->setPitch(SoundId::kSelect, 0.0F);
            SOUND->play(SoundId::kSelect, false);
        }
        else
        {
            SOUND->stop(SoundId::kSelect);
            SOUND->setPitch(SoundId::kSelect, -0.5F);
            SOUND->play(SoundId::kSelect, false);
        }
    }


    return kKeep;
}
// 続けるへ
int Pause::toContinue()
{
    using namespace std::chrono;
    using Clock = high_resolution_clock;
    using MS    = milliseconds;

    if( count_started_ == false )
    {
        count_start_ = high_resolution_clock::now();
        count_started_ = true;
        remaining_count_ = kWaitCount;
    }


    // 経過時間の計測
    auto elapsed_ms = duration_cast<MS>( Clock::now() - count_start_ );
    long new_count = kWaitCount - static_cast<long>(elapsed_ms.count() / kCountTimeMs );

    // 前回から1カウント分経過していたら
    if( new_count != remaining_count_ )
    {
        remaining_count_ = new_count;
        number_scale_ = kNumberScaleDefault;
        number_alpha_ = kNumberAlphaDefault;
    }
    // 前回から1カウント分経過していないなら
    else
    {
        long long elapsed_per_count = elapsed_ms.count() % kCountTimeMs;

        number_scale_ += static_cast<float>(elapsed_per_count) * kNumberScaleAddPerMs;
        number_alpha_ += static_cast<float>(elapsed_per_count) * kNumberAlphaAddPerMs;
    }

    remaining_count_ = kWaitCount - 
        static_cast<long>(
            duration_cast<MS>( Clock::now() - count_start_ ).count()) / 
        kCountTimeMs;


    if( remaining_count_ <= 0L )
    {
        return kContinue;
    }

    return kKeep;
}

// リトライへ
int Pause::toRestart()
{
    return kRestart;
}

// タイトルへ
int Pause::toTitle()
{
    return kTitle;
}
