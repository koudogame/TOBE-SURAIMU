
#include "result.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"

#include "easing.h"
#include "text.h"
#include "scoring.h"
#include "ranking_manager.h"
#include "title.h"
#include "endless.h"


using KeyState   = Keyboard::State;
using KeyTracker = Keyboard::KeyboardStateTracker;
using PadState   = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;


/*===========================================================================*/
constexpr unsigned kFrameWait = 10U;
constexpr long kMiniNumbersWidth = 11U;
constexpr long kMiniNumbersHeight = 16L;
constexpr long kNumbersWidth = 20L;
constexpr long kNumbersHeight = 32L;
constexpr long kCharacterWidth = 21L;
constexpr long kCharacterHeight = 32L;
enum Trimming { kBackground, kCursor, kRankIn };
const RECT kTrimming[] = 
{
    { 0L, 0L, 665L, 720L },
    { 665L, 0L, 921L, 16L },
    { 0L, 720L, 642L, 922L },
};
constexpr Vector2 kPositionBackgroundFromBase { 307.0F, 0.0F };
enum CursorPosition { kSelectSetName, kSelectOneMore, kSelectTitle, };
constexpr Vector2 kPositionCursorFromBase[] = {
    {515.0F, 287.0F},
    {515.0F, 352.0F},
    {515.0F, 417.0F},
};
constexpr Vector2 kPositionScoreFromBase { 740.0F, 148.0F };
constexpr Vector2 kPositionHeightFromBase { 417.0F, 222.0F };
constexpr Vector2 kPositionComboFromBase { 900.0F, 222.0F };
constexpr Vector2 kPositionRankInFromBase { 319.0F, 100.0F };
constexpr Vector2 kPositionRankFromBase { 900.0F, 130.0F };
constexpr Vector2 kPositionNameFromBase { 556.0F, 259.0F };

/*===========================================================================*/
Result::Result(const unsigned Rank, const Scoring Score) :
    rank_(Rank),
    score_(Score)
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
    TextureLoder* kLoader = TextureLoder::getInstance();
	texture_ = kLoader->load(L"Texture/result.png");
    if( texture_ == nullptr )        { return false; }
    texture_text_ = kLoader->load(L"Texture/result_name.png");
    if( texture_ == nullptr )        { return false; }
    texture_numbers_ = kLoader->load(L"Texture/result_score.png");
    if( texture_numbers_ == nullptr ) { return false; }
    texture_numbers_mini_ = kLoader->load(L"Texture/Rank_number.png");
    if( texture_numbers_mini_ == nullptr ) { return false; }


    // メンバ初期化
    update_ = &Result::in;
    alpha_ = 1.0F;
    count_ = 0.0F;
    for( auto& name : name_ )
    {
        name = '\0';
    }
    name_[0] = 'A';
    index_name_ = 0U;
    index_char_ = 10;
    count_frame_ = 0U;
    select_ = rank_ <= kRegisteredNum ? kSelectSetName : kSelectOneMore;
    position_base_.x = 0.0F;
    position_base_.y = -getWindowHeight<float>();

	return true;
}

/*===========================================================================*/
// 終了処理
void Result::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;

    // テクスチャ開放
    TextureLoder* kLoader = TextureLoder::getInstance();
    kLoader->release(texture_numbers_mini_);
    kLoader->release(texture_numbers_);
    kLoader->release(texture_text_);
    kLoader->release(texture_);
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
    Sprite* const kSprite = Sprite::getInstance();

    // 背景
	kSprite->draw( texture_,
        position_base_ + kPositionBackgroundFromBase,
        &kTrimming[Trimming::kBackground], alpha_ );

    // スコア
    Text::drawNumber( score_.getScore(),
        texture_numbers_,
        position_base_ + kPositionScoreFromBase,
        kNumbersWidth, kNumbersHeight,
        10U,
        alpha_ );
    // 高さ
    Text::drawNumber( static_cast<unsigned long long>(score_.getHeight()),
        texture_numbers_mini_,
        position_base_ + kPositionHeightFromBase,
        kMiniNumbersWidth, kMiniNumbersHeight,
        1U,
        alpha_ );
    // コンボ
    Text::drawNumber( score_.getMaxCombo(),
        texture_numbers_mini_,
        position_base_ + kPositionComboFromBase,
        kMiniNumbersWidth, kMiniNumbersHeight,
        1U,
        alpha_ );

    // カーソル
    kSprite->draw( texture_,
        position_base_ + kPositionCursorFromBase[select_],
        &kTrimming[Trimming::kCursor], alpha_ );


    // ランクイン時に描画するもの
    if( rank_ <= kRegisteredNum )
    {
        kSprite->draw( texture_,
            position_base_ + kPositionRankInFromBase,
            &kTrimming[Trimming::kRankIn], alpha_ );

        Text::drawString( name_,
            texture_text_, 
            position_base_ + kPositionNameFromBase,
            kCharacterWidth, kCharacterHeight,
            alpha_ );
        Text::drawNumber( rank_,
            texture_numbers_mini_,
            position_base_ + kPositionRankFromBase,
            kMiniNumbersWidth, kMiniNumbersHeight,
            1U,
            alpha_ );
    }
}

/*===========================================================================*/
// イン
SceneBase* Result::in()
{
    count_ += 0.1F;
    float offset = Easing::getInstance()->
        expo(
            getWindowHeight<float>(),
            count_,
            Easing::Mode::In
        );
    position_base_.y += offset;


    if( count_ > 1.0F )
    {
        update_ = rank_ <= kRegisteredNum ? &Result::setName : &Result::selectNext;
    }

    return this;
}

/*===========================================================================*/
// タイトルへ遷移
SceneBase* Result::outToTitle()
{
    alpha_ -= 0.01F;

    if( alpha_ < 0.0F ) { return new Title(); }
    else                { return this; }
}
// プレイへ遷移
SceneBase* Result::outToPlay()
{
    count_ -= 0.1F;
    float offset = Easing::getInstance()->
        expo(
            getWindowHeight<float>(),
            count_,
            Easing::Mode::In
        );
    position_base_.y -= offset;

    if( count_ < -1.0F ) { return new Endless(); }
    else                 { return this; }
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

        // 名前上限か、2文字目以降に' 'が入力されたら処理を変更
        if( (index_name_ >= kNameMax) ||
            (kCharTable[index_char_] == ' ' && index_name_ != 1) )
        {
            --index_name_;
            ++select_;
            update_ = &Result::selectNext;
        }
        index_char_ = kCharNum - 1;
    }
    // 選択
    else if( 
        // 新規入力か
        ( (key_tracker.pressed.Up ||
            pad_tracker.leftStickUp == PadTracker::PRESSED) ) ||
        // 一定時間の長押しで
        ( (key_state.Up || pad_state.IsLeftThumbStickUp()) && 
            ++count_frame_ >= kFrameWait ) )
    {
        count_frame_ = 0U;
        // 循環させる
        if(++index_char_ >= kCharNum ) { index_char_ = 0; }
        name_[index_name_] = kCharTable[index_char_];
    }
    else if( 
        // 新規入力か
        ( (key_tracker.pressed.Down ||
            pad_tracker.leftStickDown == PadTracker::PRESSED) ) ||
        // 一定時間の長押しで
        ( (key_state.Down || pad_state.IsLeftThumbStickDown()) &&
            ++count_frame_ >= kFrameWait ) )
    {
        count_frame_ = 0U;
        // 循環させる
        if(--index_char_ < 0 ) { index_char_ = kCharNum - 1; }
        name_[index_name_] = kCharTable[index_char_];
    }


    return this;
}

/*===========================================================================*/
// 次のモードを選択
SceneBase* Result::selectNext()
{
    KeyTracker key_tracker = Key::getInstance()->getTracker();
    PadTracker pad_tracker = Pad::getInstance()->getTracker();

    // 決定
    if( key_tracker.pressed.Enter || pad_tracker.b == PadTracker::PRESSED )
    {
        RankingManager::getInstance()->registerScore(
            name_, score_.getScore(), score_.getHeight(), score_.getMaxCombo() 
        );

        // 各項目にあった処理へ移る
        switch( select_ ) 
        {
        case kSelectSetName: update_ = &Result::setName; break;
        case kSelectOneMore: update_ = &Result::outToPlay; break;
        case kSelectTitle:   update_ = &Result::outToTitle; break;
        }
    }
    // 選択
    else if( key_tracker.pressed.Up || 
             pad_tracker.leftStickUp == PadTracker::PRESSED )
    {
        // 上限を超えないよう制御
        if( (rank_ <= kRegisteredNum && select_ > kSelectSetName) || // ランクイン時
            (select_ > kSelectOneMore) )                             // ランク外  時
        {
            --select_;
        }
    }
    else if( key_tracker.pressed.Down ||
             pad_tracker.leftStickDown == PadTracker::PRESSED ) 
    {
        // 下限を超えないよう制御
        if( select_ < kSelectTitle )
        {
            ++select_;
        }
    }

    return this;
}
