
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
#include "Sound.h"


typedef unsigned long long ULL;

using KeyState   = Keyboard::State;
using KeyTracker = Keyboard::KeyboardStateTracker;
using PadState   = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;


/*===========================================================================*/
constexpr unsigned kShowNum                 = 10U;      // 描画数
constexpr unsigned kScoreDigits             = 10U;      // スコア桁数
constexpr unsigned kHeightDigits            = 6U;       // 高さ桁数
constexpr unsigned kComboDigits             = 2U;       // コンボ桁数
constexpr unsigned kFrameWait               = 10U;      // 長押し時、待機フレーム数
constexpr long kBigNumbersWidth             = 20L;      // 大数字横幅
constexpr long kBigNumbersHeight            = 32L;      // 大数字縦幅
constexpr long kMiniNumbersWidth            = 11U;      // 小数字横幅
constexpr long kMiniNumbersHeight           = 15L;      // 小数字縦幅
constexpr long kNumbersWidth                = 20L;      // 数字横幅
constexpr long kNumbersHeight               = 32L;      // 数字縦幅
constexpr long kMiniCharacterWidth          = 12L;      // 小文字横幅
constexpr long kMiniCharacterHeight         = 16L;      // 小文字縦幅
constexpr long kCharacterWidth              = 21L;      // 文字横幅
constexpr long kCharacterHeight             = 32L;      // 文字縦幅
constexpr float kIntervalRankingElem        = 22.0F;    // ランキング要素の間
constexpr float kAmountAlphaChangeForRankIn = 0.04F;    // "RankIn"のアルファ値の変化量
constexpr float kAlphaMaxRankIn             = 1.0F;     // "RankIn"のアルファ値上限
constexpr float kAlphaMinRankIn             = 0.0F;     // "RankIn"のアルファ値下限

enum { kTrmBackground, kTrmCursor, kTrmNameCursor, kTrmRankInLine, kTrimRankIn };
const RECT kTrimming[] =
{
    {   0L,   0L, 680L, 720L }, // back
    { 665L,   0L, 921L,  16L }, // cursor
    { 683L,  85L, 700L, 141L }, // name cursor
    {   0L, 720L, 642L, 915L }, // rankin line
    {   0L, 916L,  66L, 929L }, // rankin
};

enum { kPosBackground, kPosScore, kPosHeight, kPosCombo, kPosRankInLine,
                kPosRankIn, kPosRank, kPosName, kPosNameCursor, kPosRanking };
constexpr Vector2 kPositionFromBase[] {
    { 307.0F,   0.0F }, // back
    { 740.0F, 148.0F }, // score
    { 417.0F, 222.0F }, // height
    { 898.0F, 221.0F }, // combo
    { 319.0F, 100.0F }, // rankin line
    {   0.0F,   0.0F }, // rankin
    { 890.0F, 120.0F }, // rank
    { 556.0F, 262.0F }, // name
    { 558.0F, 247.0F }, // name cursor
    { 436.0F, 456.0F }, // ranking
};

enum { kSelectSetName, kSelectOneMore, kSelectTitle, };
constexpr Vector2 kPositionCursorFromBase[] = {
    {515.0F, 287.0F}, // set name
    {515.0F, 352.0F}, // one more
    {515.0F, 417.0F}, // title
};

enum { kName,  kScore, kHeight, kCombo };
constexpr float kDrawPositionX[] = {
       472.0F, 690.0F, 790.0F,  842.0F
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
	texture_                     = kLoader->load(L"Texture/result.png");
    if( texture_                 == nullptr )   { return false; }

    texture_char_                = kLoader->load(L"Texture/result_name.png");
    if( texture_char_            == nullptr )   { return false; }

    texture_numbers_big_         = kLoader->load(L"Texture/play_rank_2.png");
    if( texture_numbers_big_     == nullptr )   { return false; }

    texture_char_mini_           = kLoader->load(L"Texture/rank_name.png");
    if( texture_char_mini_       == nullptr)    { return false; }

    texture_numbers_             = kLoader->load(L"Texture/result_score.png");
    if( texture_numbers_         == nullptr )   { return false; }

    texture_numbers_mini_        = kLoader->load(L"Texture/Rank_number.png");
    if( texture_numbers_mini_    == nullptr )   { return false; }


    // メンバ初期化
    update_ = &Result::in;
    alpha_ = 1.0F;
    count_ = 0.0F;
    name_[0] = ' ';
    for( int i = 1; i < kNameMax; ++i )
    {
        name_[i] = '\0';
    }
    index_name_ = 0U;
    index_char_ = kCharNum - 1;
    count_frame_ = 0U;
    select_ = rank_ <= kRegisteredNum ? kSelectSetName : kSelectOneMore;
    position_base_.x = 0.0F;
    position_base_.y = -getWindowHeight<float>();
    is_add_alpha_rankin_ = false;
    alpha_rankin_ = 1.0F;

	//サウンドの再生
	SOUND->stop( SoundId::kTitle );
	SOUND->play( SoundId::kTitle , true );

	return true;
}

/*===========================================================================*/
// 終了処理
void Result::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;

    // ランキングにスコアを登録( 万が一、名前入力をしないで終了していたら"NAME"にする )
    if( name_ [0] == ' ' )
    {
        name_[0] = 'N'; name_[1] = 'A'; name_[2] = 'M'; name_[3] = 'E';
    }
    RankingManager::getInstance()->registerScore(
        name_, score_.getScore(), score_.getHeight(), score_.getMaxCombo()
    );

    // テクスチャ開放
    TextureLoder* kLoader = TextureLoder::getInstance();
    kLoader->release(texture_numbers_mini_);
    kLoader->release(texture_numbers_);
    kLoader->release(texture_char_mini_);
    kLoader->release(texture_numbers_big_);
    kLoader->release(texture_char_);
    kLoader->release(texture_);
}

/*===========================================================================*/
// 更新処理
SceneBase* Result::update()
{
    // ランクインのアルファ値を変更
    if( is_add_alpha_rankin_ ){
        if( (alpha_rankin_ += kAmountAlphaChangeForRankIn) >= kAlphaMaxRankIn)
        {
            is_add_alpha_rankin_ = false;
            alpha_rankin_ = kAlphaMaxRankIn; 
        }
    }
    else{
        if( (alpha_rankin_ -= kAmountAlphaChangeForRankIn) <= kAlphaMinRankIn)
        {
            is_add_alpha_rankin_ = true;
            alpha_rankin_ = kAlphaMinRankIn;
        }

    }

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
    // プレイヤーの上に、表示数の半数より多くがいない場合、
    //                      もしくはランキング外の場合、1位が一番上になるよう調整
    if( (rank_ <= (kShowNum / 2U)) || rank_ > kRegisteredNum )
    {
        rank = 1U;
    }
    // プレイヤーの下に、表示数の半数より多くがいない場合、最下位が一番下になるよう調整
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
        // ランキングにプレイヤーを描画
        if( rank == rank_ && draw_player_ == false )
        {
            draw_player_ = true;

            position.x -= 80.0F;
            kSprite->draw( texture_, position, &kTrimming[kTrimRankIn],
                           alpha_ < alpha_rankin_ ? alpha_ : alpha_rankin_);
            position.x += 80.0F;

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
                       position_base_ + kPositionFromBase[kPosRankInLine],
					   &kTrimming[ kTrmRankInLine ] , alpha_ , 1.0F );

        // 名前
        Text::drawString( name_,
                          texture_char_,
                          position_base_ + kPositionFromBase[kPosName],
                          kCharacterWidth, kCharacterHeight, alpha_ );


        // ランク
        Text::drawNumber( rank_,
                          texture_numbers_big_,
                          position_base_ + kPositionFromBase[kPosRank],
                          kBigNumbersWidth, kBigNumbersHeight,
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

	if( count_ >= 1.0F ) { SOUND->stop( SoundId::kTitle ); return new Endless(); }
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
		SOUND->stop( SoundId::kDicision );
		SOUND->setPitch( SoundId::kDicision , 0.0F );
		SOUND->stop( SoundId::kSelect );
		SOUND->play( SoundId::kDicision , false );

		name_[ index_name_ ] = kCharTable[ index_char_ ];

        // ' 'で入力終了
		if( name_[index_name_] == ' ' )
        {
            // 何も入力がなかったら"NAME"で初期化
            if( index_name_ == 0 )
            {
                name_[0] = 'N'; name_[1] = 'A'; name_[2] = 'M'; name_[3] = 'E';
                index_name_ = 4;
            }
            else
            {
                name_[index_name_] = '\0';
            }
            ++select_;
            update_ = &Result::selectNext;
        }
		// 名前上限で入力終了
        else if( index_name_ + 1U >= kNameMax )
		{
			++select_;
			update_ = &Result::selectNext;
		}
		// 次の文字選択へ向けて初期化
		else if( name_[ 0 ] != ' ' )
		{
			// 次の文字を' 'で初期化
			++index_name_;
			index_char_ = kCharNum - 1;
			name_[ index_name_ ] = kCharTable[ index_char_ ];

		}
	}
	// 戻る
	else if( key_tracker.pressed.Back || pad_tracker.b == PadTracker::PRESSED )
	{
		SOUND->stop( SoundId::kDicision );
		SOUND->setPitch( SoundId::kDicision , -0.5F );
		SOUND->stop( SoundId::kSelect );
		SOUND->play( SoundId::kDicision , false );

		if( index_name_ >= 1U )
		{
			name_[index_name_] = '\0';
			--index_name_;
			index_char_ = Text::getCharNum( name_[ index_name_ ] );
		}
	}
	// 選択
	else if(
		// 新規入力か
		( ( key_tracker.pressed.Up ||
			pad_tracker.leftStickUp == PadTracker::PRESSED ) ) ||
		// 一定時間の長押しで
			( ( key_state.Up || pad_state.IsLeftThumbStickUp() ) &&
			  ++count_frame_ >= kFrameWait ) )
	{
		SOUND->stop( SoundId::kSelect );
		SOUND->setPitch( SoundId::kSelect , 0.0F );
		SOUND->play( SoundId::kSelect , false );

		count_frame_ = 0U;
		// 循環させる
		if( --index_char_ < 0 ) { index_char_ = kCharNum - 1; }
		name_[ index_name_ ] = kCharTable[ index_char_ ];
	}
	else if(
		// 新規入力か
		( ( key_tracker.pressed.Down ||
			pad_tracker.leftStickDown == PadTracker::PRESSED ) ) ||
		// 一定時間の長押しで
			( ( key_state.Down || pad_state.IsLeftThumbStickDown() ) &&
			  ++count_frame_ >= kFrameWait ) )
	{
		SOUND->stop( SoundId::kSelect );
		SOUND->setPitch( SoundId::kSelect , 0.0F );
		SOUND->play( SoundId::kSelect , false );

		count_frame_ = 0U;
		// 循環させる
		if( ++index_char_ >= kCharNum ) { index_char_ = 0; }
		name_[ index_name_ ] = kCharTable[ index_char_ ];
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
		SOUND->stop( SoundId::kDicision );
		SOUND->setPitch( SoundId::kDicision , 0.0F );
		SOUND->play( SoundId::kDicision , false );

		// 各項目にあった処理へ移る
		switch( select_ )
		{
			case kSelectSetName: 
                update_ = &Result::setName;

                if( (index_name_ > 0U && index_name_ < (kNameMax - 1U)) ||
                    (index_name_ == kNameMax - 1U && name_[index_name_] == '\0') ) 
                { 
                    name_[index_name_] = '\0';
                    --index_name_; 
                }
                index_char_ = Text::getCharNum(name_[index_name_]);
                                                                break;

			case kSelectOneMore: update_ = &Result::outToPlay;  break;
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
			SOUND->stop( SoundId::kSelect );
			SOUND->setPitch( SoundId::kSelect , 0.0F );
			SOUND->play( SoundId::kSelect , false );
			--select_;
		}
		else
		{
			SOUND->stop( SoundId::kSelect );
			SOUND->setPitch( SoundId::kSelect , -0.5F );
			SOUND->play( SoundId::kSelect , false );
		}
	}
	else if( key_tracker.pressed.Down ||
			 pad_tracker.leftStickDown == PadTracker::PRESSED )
	{
		// 下限を超えないよう制御
		if( select_ < kSelectTitle )
		{
			SOUND->stop( SoundId::kSelect );
			SOUND->setPitch( SoundId::kSelect , 0.0F );
			SOUND->play( SoundId::kSelect , false );
			++select_;
		}
		else
		{
			SOUND->stop( SoundId::kSelect );
			SOUND->setPitch( SoundId::kSelect , -0.5F );
			SOUND->play( SoundId::kSelect , false );
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