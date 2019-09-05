
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
constexpr float kLineHeight       = 20.0F;      // 1行の縦幅
constexpr float kMagnification    = 0.05F;      // スクロール倍率
constexpr float kMagnificationMax = 3.0F;       // 倍率上限
constexpr float kOffset           = 5.0F;       // 描画オフセット
constexpr float kOffsetMin        = -200.0F;    // オフセット下限
constexpr float kOffsetMax        = 1800.0F;    // オフセット上限
constexpr float kFieldMax         = 640.0F;     // 描画位置上限( 下 )
constexpr float kIntervalNumber   = 1.0F;       // 数字の間
constexpr long kCharWidth         = 12L;        // 文字横幅
constexpr long kCharHeight        = 16L;        // 文字縦幅
constexpr long kNumWidth          = 10L;        // 数字横幅
constexpr long kNumHeight         = 16L;        // 数字縦幅
constexpr unsigned kScoreDigits   = 10U;        // スコア桁数
constexpr unsigned kHeightDigits  = 6U;         // 高さ桁数
constexpr unsigned kComboDigits   = 2U;         // コンボ桁数

enum { kRank, kName, kScore, kHeight, kCombo, };
constexpr float kCoordinateX[] = {
    410.0F, 450.0F, 575.0F, 713.0F, 807.0F
};

constexpr Vector2 kPositionBase( 0.0F, 200.0F );
enum { kFrame, kBack, kField };
constexpr Vector2 kPosition[] = {
    { 140.0F,  21.0F },
    { 140.0F,  70.0F },
    { 680.0F, 150.0F },
};
const RECT kTrimming[] = {
    {    0L,    0L, 1145L,  681L },
    {    0L,  681L, 1145L, 1313L },
    { 1145L,    0L,  1341L,  12L },
};


constexpr float kFieldDepth = 1.0F;
constexpr float kTextDepth = 2.0F;
constexpr float kFrameDepth = 10.0F;


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

    // ランキングデータ読み込み
    RankingManager* ranking_manager = RankingManager::getInstance();
    for( unsigned i = 1U; i <= kRegisteredNum; ++i )
    {
        data_.push_back( ranking_manager->getData(i) );
    }

    // メンバ初期化
    magnification_ = 1.0F;
    offset_ = 0.0F;
    sort_mode_ = kScore;

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
		pad_tracker.leftStickUp == PadTracker::PRESSED ||
        pad_tracker.dpadUp      == PadTracker::PRESSED )
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
			 pad_tracker.leftStickDown == PadTracker::PRESSED ||
             pad_tracker.dpadDown      == PadTracker::PRESSED )
	{
		magnification_ = 1.0F;
		addOffset( &offset_ , kLineHeight );
		sound_flag_ = false;

		SOUND->setPitch( SoundId::kSelect , 1.0F );
		SOUND->stop( SoundId::kSelect );
		SOUND->play( SoundId::kSelect , true );
	}
	// 上長押しでスクロール( 押している間スクロールスクロール倍率を上げる )
	else if( key_state.Up ||
             pad_state.IsLeftThumbStickUp() ||
             pad_state.dpad.up )
	{
		addOffset( &offset_ , -kOffset * magnification_ );
		addMagnification( &magnification_ );
	}
	// 下長押しでスクロール( 押している間スクロールスクロール倍率を上げる )
	else if( key_state.Down || 
             pad_state.IsLeftThumbStickDown() ||
             pad_state.dpad.down )
	{
		addOffset( &offset_ , kOffset * magnification_ );
		addMagnification( &magnification_ );
	}
    // LeftかLでソートの条件を一つ前に
    else if( key_tracker.pressed.Left || pad_tracker.leftShoulder == PadTracker::PRESSED )
    {
        // スクロールを初期化
        offset_ = 0.0F;

        if( --sort_mode_ < kScore ) { sort_mode_ = kCombo; }
        sort( sort_mode_ );
    }
    // RightかRでソートの条件を一つ後に
    else if( key_tracker.pressed.Right || pad_tracker.rightShoulder == PadTracker::PRESSED )
    {
        // スクロールを初期化
        offset_ = 0.0F;

        if( ++sort_mode_ > kCombo ) { sort_mode_ = kScore; }
        sort( sort_mode_ );
    }
	// Spaceかaボタンが離されたら決定
	else if( key_tracker.released.Space || pad_tracker.a == PadTracker::RELEASED )
	{
		SOUND->stop( SoundId::kSelect );
		SOUND->stop( SoundId::kDicision );
		SOUND->play( SoundId::kDicision , false );

		return new Title();
	}
	else if((key_tracker.released.Up ||
			 key_tracker.released.Down ||
			 pad_tracker.leftStickUp   == PadTracker::RELEASED ||
			 pad_tracker.leftStickDown == PadTracker::RELEASED ||
             pad_tracker.dpadUp        == PadTracker::RELEASED ||
             pad_tracker.dpadDown      == PadTracker::RELEASED) && !sound_flag_ )
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
	kSprite->reserveDraw( texture_ , kPosition[ kBack ] , kTrimming[ kBack ] , 1.0F , 0.2F );

    // ランキングの描画
    Vector2 position = kPositionBase;
    position.y -= offset_;
    int rank = 0;
    for( auto data : data_ )
    {
        ++rank;
        if( position.y < kPosition[kField].y ) { position.y += kLineHeight; continue;}
        if( position.y > kFieldMax )           { break; }

        // フィールド
        position.x = kPosition[kField].x;
        position.y += 3.0F;
		kSprite->reserveDraw( texture_ , position , kTrimming[ kField ] , 1.0F , 0.3F );
        position.y -= 3.0F;

        // ランク
        position.x = kCoordinateX[kRank] + kNumWidth *
                            ( rank > 99U ? 3.0F : (rank > 9U ? 2.5F :  2.0F) );
        Text::drawNumber( rank, texture_numbers_, position,
                          kNumWidth, kNumHeight, 1U, 1.0F, 0.0F, kTextDepth );

        // 名前
        position.x = kCoordinateX[kName];
        Text::drawString( data.name, texture_characters_, position,
                          kCharWidth, kCharHeight, 1.0F, 0.0F, kTextDepth );

        // スコア
        position.x = kCoordinateX[kScore] + kNumWidth * kScoreDigits;
        Text::drawNumber( data.score, texture_numbers_, position,
                          kNumWidth, kNumHeight, kScoreDigits, 1.0F, kIntervalNumber, kTextDepth );

        // 高さ
        position.x = kCoordinateX[kHeight] + kNumWidth * kHeightDigits;
        Text::drawNumber( static_cast<ULL>(data.height), texture_numbers_,
                          position,
                          kNumWidth, kNumHeight, 1U, 1.0F, kIntervalNumber, kTextDepth );

        // コンボ
        position.x = kCoordinateX[kCombo] + kNumWidth * kComboDigits;
        Text::drawNumber( data.combo, texture_numbers_, position,
                         kNumWidth, kNumHeight, 1U, 1.0F, kIntervalNumber, kTextDepth );

        position.y += kLineHeight;
    }


    // フレームの描画
	kSprite->reserveDraw( texture_, kPosition[kFrame], kTrimming[kFrame], 1.0F, kFrameDepth );
}


/*===========================================================================*/
// ソート
void Ranking::sort( const int Mode )
{
    data_.sort(
        Mode == kScore ?
        [](const RankingManager::Data& a, const RankingManager::Data& b)->bool
        {
            if( a.score == b.score ) { return a.height > b.height; }
            else                     { return a.score  > b.score; }
        } :
        Mode == kHeight ?
        [](const RankingManager::Data& a, const RankingManager::Data& b)->bool
        {
            if( a.height == b.height ) { return a.score > b.score; }
            else                       { return a.height > b.height; }
        } :
        /* Mode == kCombo */
        [](const RankingManager::Data& a, const RankingManager::Data& b)->bool
        {
            if( a.combo == b.combo ) { return a.score > b.score; }
            else                     { return a.combo > b.combo; }
        }
    );
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

