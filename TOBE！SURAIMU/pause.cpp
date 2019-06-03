
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

constexpr long kWidth        = 270L;
constexpr long kHeight       = 270L;
constexpr long kCursorWidth  = 270L;
constexpr long kCursorHeight = 16L;
constexpr float kCursorMove  = 48.0F;

const Vector2 kPosition( kWindowWidthHarf - (kWidth  / 2.0F),
                         (kWindowHeight / 2.0F) - (kHeight / 2.0F) );
const Vector2 kPositionCursorStart( kWindowWidthHarf - (kCursorWidth / 2.0F),
                                    345.0F );
enum { kPause, kBack, kCursor };
const RECT kTrimming[] = {
    {   0L,   0L,  270L, 270L },
    { 270L,   0L, 1550L, 720L },
    {   0L, 270L,  269L, 289L },
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
    KeyTracker key = Key::getInstance()->getTracker();
    PadTracker pad = Pad::getInstance()->getTracker();

    // 決定
    if( pad.a == PadTracker::RELEASED || key.released.Space )
    {
		SOUND->stop( SoundId::kDicision );
		SOUND->play( SoundId::kDicision , false );
        return select_;
    }
    // 上選択
    else if( pad.leftStickUp == PadTracker::PRESSED || key.pressed.Up )
    {
        if( select_ > kContinue )
        {
            --select_;
            position_cursor_.y -= kCursorMove;
			SOUND->stop( SoundId::kSelect );
			SOUND->setPitch( SoundId::kSelect , 0.0F );
			SOUND->play( SoundId::kSelect , false );
        }
		else
		{
			SOUND->stop( SoundId::kSelect );
			SOUND->setPitch( SoundId::kSelect , -0.5F );
			SOUND->play( SoundId::kSelect , false );
		}
    }
    // 下選択
    else if( pad.leftStickDown == PadTracker::PRESSED || key.pressed.Down )
    {
        if( select_ < kTitle )
        {
            ++select_;
            position_cursor_.y += kCursorMove;
			SOUND->stop( SoundId::kSelect );
			SOUND->setPitch( SoundId::kSelect , 0.0F );
			SOUND->play( SoundId::kSelect , false );
        }
		else
		{
			SOUND->stop( SoundId::kSelect );
			SOUND->setPitch( SoundId::kSelect , -0.5F );
			SOUND->play( SoundId::kSelect , false );
		}
    }


    return kKeep;
}

/*===========================================================================*/
// 描画処理
void Pause::draw() const
{
    Sprite* kSprite = Sprite::getInstance();

    // 背景
	kSprite->draw( texture_ , Vector2::Zero ,  &kTrimming[kBack] , 1.0F , 1.0F );

    // ポーズ本体
    kSprite->draw( texture_, kPosition,        &kTrimming[kPause], 1.0F, 1.0F );

    // カーソル
    kSprite->draw( texture_, position_cursor_, &kTrimming[kCursor], 1.0F, 1.0F );

}

/*===========================================================================*/
// メンバリセット
void Pause::reset()
{
    select_ = kContinue;
    position_cursor_ = kPositionCursorStart;
}
