
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


typedef unsigned long long ULL;

using KeyState   = Keyboard::State;
using KeyTracker = Keyboard::KeyboardStateTracker;
using PadState   = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;


/*===========================================================================*/
constexpr unsigned kShowNum          = 10U;
constexpr unsigned kScoreDigits      = 10U;
constexpr unsigned kHeightDigits     = 6U;
constexpr unsigned kComboDigits      = 2U;
constexpr unsigned kFrameWait        = 10U;
constexpr long kMiniNumbersWidth     = 11U;
constexpr long kMiniNumbersHeight    = 15L;
constexpr long kNumbersWidth         = 20L;
constexpr long kNumbersHeight        = 32L;
constexpr long kMiniCharacterWidth   = 12L;
constexpr long kMiniCharacterHeight  = 16L;
constexpr long kCharacterWidth       = 21L;
constexpr long kCharacterHeight      = 32L;
constexpr float kIntervalRankingElem = 22.0F;
enum { kTrmBackground, kTrmCursor, kTrmNameCursor, kTrmRankIn };
const RECT kTrimming[] =
{
    {   0L,   0L, 680L, 720L },
    { 665L,   0L, 921L,  16L },
    { 683L,  85L, 700L, 141L },
    {   0L, 720L, 642L, 922L },
};

enum { kPosBackground, kPosScore, kPosHeight, kPosCombo, kPosRankIn,
                kPosRank, kPosName, kPosNameCursor, kPosRanking };
constexpr Vector2 kPositionFromBase[] {
    { 307.0F,   0.0F }, // back
    { 740.0F, 148.0F }, // score
    { 417.0F, 222.0F }, // height
    { 898.0F, 221.0F }, // combo
    { 319.0F, 100.0F }, // rankin
    { 885.0F, 130.0F }, // rank
    { 556.0F, 262.0F }, // name
    { 558.0F, 247.0F }, // name cursor
    { 436.0F, 456.0F }, // ranking
};

enum { kSelectSetName, kSelectOneMore, kSelectTitle, };
constexpr Vector2 kPositionCursorFromBase[] = {
    {515.0F, 287.0F},
    {515.0F, 352.0F},
    {515.0F, 417.0F},
};

enum { kName, kScore, kHeight, kCombo };
constexpr float kDrawPositionX[] = {
    472.0F, 690.0F, 790.0F, 842.0F
};


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
    TextureLoder*           kLoader = TextureLoder::getInstance();
	texture_ =              kLoader->load(L"Texture/result.png");
    if( texture_              == nullptr )        { return false; }
    texture_char_ =         kLoader->load(L"Texture/result_name.png");
    if( texture_char_         == nullptr )        { return false; }
    texture_char_mini_ =    kLoader->load(L"Texture/rank_name.png");
    if( texture_char_mini_    == nullptr)         { return false; }
    texture_numbers_ =      kLoader->load(L"Texture/result_score.png");
    if( texture_numbers_      == nullptr )        { return false; }
    texture_numbers_mini_ = kLoader->load(L"Texture/Rank_number.png");
    if( texture_numbers_mini_ == nullptr )        { return false; }


    // メンバ初期化
    update_ = &Result::in;
    alpha_ = 1.0F;
    count_ = 0.0F;
    name_[0] = 'N'; name_[1] = 'A'; name_[2] = 'M';name_[3] = 'E';
    for( int i = 4; i < kNameMax; ++i )
    {
        name_[i] = '\0';
    }
    index_name_ = 4U;
    index_char_ = kCharNum - 1;
    count_frame_ = 0U;
    select_ = rank_ <= kRegisteredNum ? kSelectSetName : kSelectOneMore;
    position_base_.x = 0.0F;
    position_base_.y = -getWindowHeight<float>();

	//サウンドの取得
    AudioLoader* kAudio = AudioLoader::getInstance();
	bgm_ = kAudio->getSound( L"Sound/title2-dova.wav" );
	select_se_ = kAudio->getSound( L"Sound/select1-dova.wav" );
	decision_se_ = kAudio->getSound( L"Sound/select2-dova.wav" );
	bgm_->allReset();
	bgm_->play( AudioContainer::Mode::kDefault , true );
	return true;
}

/*===========================================================================*/
// 終了処理
void Result::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;

    // ランキングにスコアを登録
    RankingManager::getInstance()->registerScore(
        name_, score_.getScore(), score_.getHeight(), score_.getMaxCombo()
    );

    // テクスチャ開放
    TextureLoder* kLoader = TextureLoder::getInstance();
    kLoader->release(texture_numbers_mini_);
    kLoader->release(texture_numbers_);
    kLoader->release(texture_char_mini_);
    kLoader->release(texture_char_);
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
                   position_base_ + kPositionFromBase[kPosBackground],
				   &kTrimming[ kTrmBackground ] , alpha_ , 0.9F );

    // スコア
    Text::drawNumber( score_.getScore(),
                      texture_numbers_,
                      position_base_ + kPositionFromBase[kPosScore],
                      kNumbersWidth, kNumbersHeight,
                      10U, alpha_ );

    // 高さ
    Text::drawNumber( static_cast<ULL>(score_.getHeight()),
                      texture_numbers_mini_,
                      position_base_ + kPositionFromBase[kPosHeight],
                      kMiniNumbersWidth, kMiniNumbersHeight, 1U, alpha_ );

    // コンボ
    Text::drawNumber( score_.getMaxCombo(), texture_numbers_mini_,
                      position_base_ + kPositionFromBase[kPosCombo],
                      kMiniNumbersWidth, kMiniNumbersHeight, 1U, alpha_ );

    // カーソル
    kSprite->draw( texture_,
                   position_base_ + kPositionCursorFromBase[select_],
				   &kTrimming[ kTrmCursor ] , alpha_ , 1.0F );

    // ランキング
    RankingManager* kRanking = RankingManager::getInstance();
    RankingManager::Data data;
    Vector2 position = position_base_ + kPositionFromBase[kPosRanking];

    // ランキングの描画順位を決定
    unsigned rank = 0U;

    // プレイヤーの上に、表示数の半数がいない場合、1位が一番上になるよう調整
    if( (rank_ < (kShowNum / 2U)) || rank_ > kRegisteredNum )
    {
        rank = 1U;
    }
    // プレイヤーの下に、表示数の半数がいない場合、最下位が一番下になるよう調整
    else if( rank_ > kRegisteredNum - (kShowNum / 2U) ) 
    {
        rank = kRegisteredNum - kShowNum;
    }
    // プレイヤーを中心に位置を調整
    else
    {
        rank = rank_ - (kShowNum / 2U); 
    }

    bool draw_player_ = false;
    for( int i = 0; i <= kShowNum; ++i, ++rank )
    {

        if( rank == rank_ && draw_player_ == false )
        {
            draw_player_ = true;
            drawRankingElem(
                position,
                rank, name_,
                score_.getScore(), score_.getHeight(), score_.getMaxCombo() );
            position.y += kIntervalRankingElem;
            ++i;
        }


        data = kRanking->getData(rank);
        drawRankingElem(
            position,
            draw_player_ ? rank + 1 : rank, data.name.c_str(),
            data.score, data.height, data.combo );

        position.y += kIntervalRankingElem;
    }

    // ランクイン時に描画するもの
    if( rank_ <= kRegisteredNum )
    {
        // 下線
        kSprite->draw( texture_,
                       position_base_ + kPositionFromBase[kPosRankIn],
					   &kTrimming[ kTrmRankIn ] , alpha_ , 1.0F );

        // 名前
        Text::drawString( name_,
                          texture_char_,
                          position_base_ + kPositionFromBase[kPosName],
                          kCharacterWidth, kCharacterHeight, alpha_ );


        // ランク
        Text::drawNumber( rank_,
                          texture_numbers_mini_,
                          position_base_ + kPositionFromBase[kPosRank],
                          kMiniNumbersWidth, kMiniNumbersHeight,
                          1U, alpha_ );

        // 名前カーソル( 名前選択時にのみ描画 )
        if( update_ == &Result::setName )
        {
            position = position_base_ + kPositionFromBase[kPosNameCursor];
            position.x += kCharacterWidth * index_name_;
            kSprite->draw( texture_,
                           position,
                           &kTrimming[kTrmNameCursor], alpha_, 1.0F
			);
        }
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
    count_ += 0.1F;
    float offset = Easing::getInstance()->
        expo(
            getWindowHeight<float>(),
            count_,
            Easing::Mode::Out
        );
    position_base_.y -= offset;
	bgm_->setVolume( 1.0F - count_ );

	if( count_ >= 1.0F ) { bgm_->stop(); return new Endless(); }
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
    if( key_tracker.released.Space || pad_tracker.a == PadTracker::RELEASED )
    {
		decision_se_->resetPitch();
		decision_se_->stop();
		decision_se_->play( AudioContainer::Mode::kDefault );

        name_[index_name_] = kCharTable[index_char_];

        // 名前上限か、2文字目以降に' 'が入力されたら処理を変更
        if( (index_name_ + 1U >= kNameMax) ||
            (kCharTable[index_char_] == ' ' && index_name_ != 0) )
        {
            ++select_;
            update_ = &Result::selectNext;
        }
        // 次の文字選択へ向けて初期化
        else if( name_[0] != ' ')
        {
            // 次の文字を'A'で初期化
            ++index_name_;
            index_char_ = kCharNum - 1;
            name_[index_name_] = kCharTable[index_char_];

        }
    }
    // 戻る
    else if( key_tracker.pressed.Back || pad_tracker.b == PadTracker::PRESSED )
    {
		if( index_name_ >= 1U )
		{
			decision_se_->setPitch( -1.0F );
			decision_se_->stop();
			decision_se_->play( AudioContainer::Mode::kDefault );
			name_[ index_name_ ] = ' ';
			--index_name_;
			index_char_ = Text::getCharNum( name_[ index_name_ ] );
		}
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
		select_se_->stop();
		select_se_->resetPitch();
		select_se_->play( AudioContainer::Mode::kDefault );
        count_frame_ = 0U;
        // 循環させる
        if(--index_char_ < 0 ) { index_char_ = kCharNum - 1; }
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
		select_se_->stop();
		select_se_->resetPitch();
		select_se_->play( AudioContainer::Mode::kDefault );
        count_frame_ = 0U;
        // 循環させる
        if(++index_char_ >= kCharNum ) { index_char_ = 0; }
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
    if( key_tracker.released.Space || pad_tracker.a == PadTracker::RELEASED )
    {
		decision_se_->resetPitch();
		decision_se_->stop();
		decision_se_->play( AudioContainer::Mode::kDefault );
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
		if( ( rank_ <= kRegisteredNum && select_ > kSelectSetName ) || // ランクイン時
			( select_ > kSelectOneMore ) )                             // ランク外  時
		{
			--select_;
			select_se_->stop();
			select_se_->resetPitch();
			select_se_->play( AudioContainer::Mode::kDefault );
		}
		else
		{
			select_se_->stop();
			select_se_->setPitch( -0.5F );
			select_se_->play( AudioContainer::Mode::kDefault );
		}
	}
	else if( key_tracker.pressed.Down ||
			 pad_tracker.leftStickDown == PadTracker::PRESSED )
	{
		// 下限を超えないよう制御
		if( select_ < kSelectTitle )
		{
			++select_;
			select_se_->stop();
			select_se_->resetPitch();
			select_se_->play( AudioContainer::Mode::kDefault );
		}
		else
		{
			select_se_->stop();
			select_se_->setPitch( -0.5F );
			select_se_->play( AudioContainer::Mode::kDefault );
		}
	}

    return this;
}


/*===========================================================================*/
// ランキングの要素を描画
void Result::drawRankingElem( Vector2 Position,
                      const unsigned Rank, const char* Name,
                      const unsigned long long Score,
                      const double Height, const unsigned Combo )
{
    // ランク ( 桁数に応じて位置を変える ) /*     3桁     */　　/*    2桁    */   /*1桁*/
    Position.x += kMiniNumbersWidth * (Rank > 99U ? 3.0F : (Rank > 9 ? 2.5F : 2.0F));
    Text::drawNumber( Rank,
        texture_numbers_mini_,
        Position, kMiniNumbersWidth, kMiniNumbersHeight,
        1U, alpha_ );

    // 名前
    Position.x = kDrawPositionX[kName];
    Text::drawString( Name,
        texture_char_mini_,
        Position, kMiniCharacterWidth, kMiniCharacterHeight,
        alpha_ );

    // スコア
    Position.x = kDrawPositionX[kScore];
    Text::drawNumber( Score,
        texture_numbers_mini_,
        Position, kMiniNumbersWidth, kMiniNumbersHeight,
        kScoreDigits, alpha_ );

    // 距離
    Position.x = kDrawPositionX[kHeight];
    Text::drawNumber( static_cast<ULL>(Height),
        texture_numbers_mini_,
        Position, kMiniNumbersWidth, kMiniNumbersHeight,
        kHeightDigits, alpha_ );

    // 最大コンボ
    Position.x = kDrawPositionX[kCombo];
    Text::drawNumber( Combo,
        texture_numbers_mini_,
        Position, kMiniNumbersWidth, kMiniNumbersHeight,
        kComboDigits, alpha_ );

}