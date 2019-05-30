
// 板場 温樹

#include "ranking.h"

#include "textureLoder.h"
#include "sprite.h"
#include "key.h"
#include "pad.h"

#include "ranking_manager.h"
#include "text.h"

#include "title.h"
#include "Sound.h"

typedef unsigned long long ULL;

using KeyState = Keyboard::State;
using KeyTracker = Keyboard::KeyboardStateTracker;
using PadState = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;

void addMagnification(float* const Val);
void addOffset(float* const Val, const float AddVal);

/*===========================================================================*/
constexpr float kLineHeight       = 20.0F;
constexpr float kMagnification    = 0.1F;
constexpr float kMagnificationMax = 2.0F;
constexpr float kOffset           = 5.0F;
constexpr float kOffsetMin        = -200.0F;
constexpr float kOffsetMax        = 1800.0F;
constexpr float kFieldMax         = 640.0F;
constexpr float kIntervalNumber   = 1.0F;
constexpr long kCharWidth         = 12L;
constexpr long kCharHeight        = 16L;
constexpr long kNumWidth          = 10L;
constexpr long kNumHeight         = 16L;
constexpr unsigned kScoreDigits   = 10U;
constexpr unsigned kHeightDigits  = 6U;
constexpr unsigned kComboDigits   = 2U;

enum { kRank, kName, kScore, kHeight, kCombo, };
constexpr float kCoordinateX[] = {
    390.0F, 430.0F, 535.0F, 673.0F, 767.0F
};

constexpr Vector2 kPositionBase( 0.0F, 200.0F );
enum { kFrame, kBack, kField };
constexpr Vector2 kPosition[] = {
    { 120.0F,  21.0F },
    { 370.0F,  43.0F },
    { 638.0F, 200.0F },
};
const RECT kTrimming[] = {
    {    0L,    0L, 1133L,  678L },
    { 1133L,    0L, 1674L,  635L },
    {    0L,  678L,  196L,   690L },
};


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
    TextureLoder* kLoader = TextureLoder::getInstance();
    texture_ = kLoader->load(L"Texture/ranking.png");
    if( texture_ == nullptr ) { return false; }
    texture_numbers_ = kLoader->load(L"Texture/ranking_number.png");
    if( texture_numbers_ == nullptr ) {return false; }
    texture_characters_ = kLoader->load(L"Texture/rank_name.png");
    if( texture_characters_ == nullptr ) { return false; }


    // メンバ初期化
    magnification_ = 1.0F;
    offset_ = 0.0F;

	sound_flag_ = false;

    return true;
}

/*===========================================================================*/
// 終了処理
void Ranking::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;

    // テクスチャ開放
    TextureLoder* kLoader = TextureLoder::getInstance();
    kLoader->release( texture_characters_);
    kLoader->release( texture_numbers_);
    kLoader->release( texture_ );
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
		addOffset( &offset_ , -kLineHeight );
		sound_flag_ = false;

		SOUND->setPitch( SoundId::kSelect , 1.0F );
		SOUND->stop( SoundId::kSelect );
		SOUND->play( SoundId::kSelect , true );
	}
	// 下新規入力時、1列分下へスクロール
	else if( key_tracker.pressed.Down ||
			 pad_tracker.leftStickDown == PadTracker::PRESSED )
	{
		magnification_ = 1.0F;
		addOffset( &offset_ , kLineHeight );
		sound_flag_ = false;

		SOUND->setPitch( SoundId::kSelect , 1.0F );
		SOUND->stop( SoundId::kSelect );
		SOUND->play( SoundId::kSelect , true );
	}
	// 上長押しでスクロール( 押している間スクロールスクロール倍率を上げる )
	else if( key_state.Up || pad_state.IsLeftThumbStickUp() )
	{
		addOffset( &offset_ , -kOffset * magnification_ );
		addMagnification( &magnification_ );
	}
	// 下長押しでスクロール( 押している間スクロールスクロール倍率を上げる )
	else if( key_state.Down || pad_state.IsLeftThumbStickDown() )
	{
		addOffset( &offset_ , kOffset * magnification_ );
		addMagnification( &magnification_ );
	}
	// Spaceかaボタンが離されたら決定
	else if( key_tracker.released.Space || pad_tracker.a == PadTracker::RELEASED )
	{
		SOUND->stop( SoundId::kSelect );
		SOUND->stop( SoundId::kDicision );
		SOUND->play( SoundId::kDicision , false );

		return new Title();
	}
	else if( (key_tracker.released.Up ||
			 pad_tracker.leftStickUp == PadTracker::RELEASED ||
			 key_tracker.released.Down ||
			 pad_tracker.leftStickDown == PadTracker::RELEASED) && !sound_flag_ )
	{
		SOUND->stop( SoundId::kSelect );
		SOUND->play( SoundId::kSelect , false );
		sound_flag_ = false;
	}



	if( ( offset_ == kOffsetMax || offset_ == kOffsetMin ) && !sound_flag_ )
	{
		sound_flag_ = true;

		SOUND->setPitch( SoundId::kSelect , -0.5F );
		SOUND->stop( SoundId::kSelect );
		SOUND->play( SoundId::kSelect , false );
	}

	return this;
}

/*===========================================================================*/
// 描画処理
void Ranking::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    // バックの描画
	kSprite->draw( texture_ , kPosition[ kBack ] , &kTrimming[ kBack ] , 1.0F , 0.2F );

    // ランキングの描画
    RankingManager* const kRanking = RankingManager::getInstance();
    RankingManager::Data data;
    Vector2 position = kPositionBase;
    position.y -= offset_;
    for( unsigned rank = 1U; rank <= kRegisteredNum;
                                            ++rank, position.y += kLineHeight )
    {
        if( position.y < kPosition[kField].y ) { continue; }
        if( position.y > kFieldMax )           { break; }

        data = kRanking->getData( rank );

        // フィールド
        position.x = kPosition[kField].x;
        position.y += 3.0F;
		kSprite->draw( texture_ , position , &kTrimming[ kField ] , 1.0F , 0.3F );
        position.y -= 3.0F;

        // ランク
        position.x = kCoordinateX[kRank] + kNumWidth *
                            ( rank > 99U ? 3.0F : (rank > 9U ? 2.5F :  2.0F) );
        Text::drawNumber( rank, texture_numbers_, position,
                          kNumWidth, kNumHeight );

        // 名前
        position.x = kCoordinateX[kName];
        Text::drawString( data.name, texture_characters_, position,
                          kCharWidth, kCharHeight );

        // スコア
        position.x = kCoordinateX[kScore] + kNumWidth * kScoreDigits;
        Text::drawNumber( data.score, texture_numbers_, position,
                          kNumWidth, kNumHeight, kScoreDigits, 1.0F, kIntervalNumber );

        // 高さ
        position.x = kCoordinateX[kHeight] + kNumWidth * kHeightDigits;
        Text::drawNumber( static_cast<ULL>(data.height), texture_numbers_,
                          position,
                          kNumWidth, kNumHeight, 1U, 1.0F, kIntervalNumber );

        // コンボ
        position.x = kCoordinateX[kCombo] + kNumWidth * kComboDigits;
        Text::drawNumber( data.combo, texture_numbers_, position,
                         kNumWidth, kNumHeight, 1U, 1.0F, kIntervalNumber );
    }


    // フレームの描画
	kSprite->draw( texture_ , kPosition[ kFrame ] , &kTrimming[ kFrame ] , 1.0F , 1.0F );
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

	if( *Val > kOffsetMax ) { *Val = kOffsetMax;}
    else if( *Val < kOffsetMin )  { *Val = kOffsetMin; }
}
