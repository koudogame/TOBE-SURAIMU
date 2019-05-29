
#include "pause.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"


using KeyTracker = Keyboard::KeyboardStateTracker;
using PadTracker = GamePad::ButtonStateTracker;


/*===========================================================================*/
constexpr long kWidth = 270L;
constexpr long kHeight = 270L;
constexpr long kCursorWidth = 270L;
constexpr long kCursorHeight = 16L;
constexpr float kCursorMove = 48.0F;
const Vector2 kPosition( getWindowWidth<float>() / 2.0F - kWidth / 2.0F,
                         getWindowHeight<float>() / 2.0F - kHeight / 2.0F );
const Vector2 kPositionCursorStart( getWindowWidth<float>() / 2.0F - kCursorWidth / 2.0F,
                                    345.0F );

const RECT kTrimming{ 0L, 0L, 270L, 270L };
const RECT kTrimmingCursor{ 0L, 270L, 269L, 289L };

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

    texture_      = TextureLoder::getInstance()->load(L"Texture/pause.png");
    if( texture_ == nullptr )      { return false; }
    texture_back_ = TextureLoder::getInstance()->load(L"Texture/pause_back.png");
    if( texture_back_ == nullptr ) { return false; }

    reset();

    created_ = true;

	select_se_[ 0 ] = AudioLoader::getInstance()->getSound( L"Sound/select1-dova.wav" );
	select_se_[ 1 ] = AudioLoader::getInstance()->getSound( L"Sound/select2-dova.wav" );
    return true;
}

/*===========================================================================*/
// 終了処理
void Pause::destroy()
{
    if( created_ == false ) { return; }

    if( texture_back_ ) { TextureLoder::getInstance()->release(texture_back_);}
    if( texture_ )      { TextureLoder::getInstance()->release(texture_); }
}

/*===========================================================================*/
// 更新処理
int Pause::update()
{
    KeyTracker key = Key::getInstance()->getTracker();
    PadTracker pad = Pad::getInstance()->getTracker();

    // 決定
    if( key.released.Space || pad.a == PadTracker::RELEASED )
    {
		select_se_[ 1 ]->stop();
		select_se_[ 1 ]->play( AudioContainer::Mode::kDefault );
        return select_;
    }
    else if( key.pressed.Up || pad.leftStickUp == PadTracker::PRESSED )
    {
        if( select_ > kContinue )
        {
            --select_;
            position_cursor_.y -= kCursorMove;
			select_se_[ 0 ]->stop();
			select_se_[ 0 ]->resetPitch();
			select_se_[ 0 ]->play( AudioContainer::Mode::kDefault );
        }
		else
		{
			select_se_[ 0 ]->stop();
			select_se_[ 0 ]->setPitch( -0.5F );
			select_se_[ 0 ]->play( AudioContainer::Mode::kDefault );
		}
    }
    else if( key.pressed.Down || pad.leftStickDown == PadTracker::PRESSED )
    {
        if( select_ < kTitle )
        {
            ++select_;
            position_cursor_.y += kCursorMove;

			select_se_[ 0 ]->stop();
			select_se_[ 0 ]->resetPitch();
			select_se_[ 0 ]->play( AudioContainer::Mode::kDefault );
        }
		else
		{
			select_se_[ 0 ]->stop();
			select_se_[ 0 ]->setPitch( -0.5F );
			select_se_[ 0 ]->play( AudioContainer::Mode::kDefault );
		}
    }


    return kKeep;
}

/*===========================================================================*/
// 描画処理
void Pause::draw() const
{
    Sprite* kSprite = Sprite::getInstance();

	kSprite->draw( texture_back_ , Vector2::Zero , nullptr , 1.0F , 1.0F );

    kSprite->draw( texture_, kPosition, &kTrimming, 1.0F, 1.0F );

    kSprite->draw( texture_, position_cursor_, &kTrimmingCursor, 1.0F, 1.0F );

}

/*===========================================================================*/
// リセット
void Pause::reset()
{
    select_ = kContinue;
    position_cursor_ = kPositionCursorStart;
}
