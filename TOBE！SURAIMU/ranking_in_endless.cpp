
// 板場 温樹

#include "ranking_in_endless.h"

#include "textureLoder.h"
#include "sprite.h"

#include "task_manager.h"
#include "text.h"


// ランク : 1 ~ 101

/*===========================================================================*/
constexpr float kWindowWidth  = getWindowWidth<float>();
constexpr float kWindowHeight = getWindowHeight<float>();

constexpr long kNumberWidth               = 11L;                        // 数字横幅
constexpr long kCharacterWidth            = 12L;                        // 文字横幅
constexpr long kTextHeight                = 16L;                        // テキスト縦幅
constexpr long kPlayerRankWidth           = 20L;                        // プレイヤーランク横幅
constexpr long kPlayerRankHeight          = 32L;                        // プレイヤーランク縦幅
constexpr long kLineHeight                = 25L;                        // 1行の高さ
constexpr char kPlayerName[]              = "YOU";                      // プレイヤー初期名前
constexpr long kDispTimeMSPF              = 250L / 16L;                 // ランキング上昇にかかる時間( ミリ秒/フレーム )
constexpr float kBarPositionX           = 960.0F;                       // 下線x座標
constexpr float kIntervalRankToName       = kCharacterWidth * 2.0F;     // ランクと名前の間
constexpr float kIntervalNameToBar        = -5.0F;                      // 名前と下線の間
const float kDrawPositionXScore           = kWindowWidth - 23.0F;       // スコア描画位置X
const unsigned kScoreDigits               = 10U;                        // スコア桁数
constexpr Vector2 kBasePosition = { 1020.0F, kWindowHeight / 2.0F };    // 基本位置
constexpr Vector2 kDeltaPositionPlayerRank = { 3.0F, 9.0F };            // プレイヤーランクの差異

enum { kNormalBar, kPlayerBar };
const RECT kTrimming[] = {
    { 0L,  0L, 415L, 24L },
    { 0L, 24L, 415L, 48L }
};

/*===========================================================================*/
RankingInEndless::RankingInEndless() :
    ranking_(kRegisteredNum)
{
}

RankingInEndless::~RankingInEndless()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool RankingInEndless::init()
{
    destroy();
    created_ = true;

    // テクスチャの読み込み
    TextureLoder* kLoader = TextureLoder::getInstance();

    texture_                     = kLoader->load(L"Texture/Pray_Rank_Shadow.png");
    if( texture_                 == nullptr ) { return false; }

    texture_bar_                 = kLoader->load(L"Texture/mini_ranking_bar.png");
    if( texture_bar_             == nullptr ) { return false; }
    
    texture_number_              = kLoader->load(L"Texture/Rank_Number.png");
    if( texture_number_          == nullptr ) { return false; }

    texture_number_forplayer_    = kLoader->load(L"Texture/play_rank_2.png");
    if(texture_number_forplayer_ == nullptr ) { return false; }

    texture_text_                = kLoader->load(L"Texture/Rank_Name.png");
    if( texture_text_            == nullptr ) { return false; }


    TaskManager::getInstance()->registerTask(this, TaskUpdate::kRanking);
    TaskManager::getInstance()->registerTask(this, TaskDraw::kObject);


    // ランキング情報を取得
    RankingManager::Data data;
    RankingManager* kRanking = RankingManager::getInstance();
    for( unsigned i = 0; i < kRegisteredNum; ++i)
    {
        data = kRanking->getData( i + 1U );
        ranking_[i].rank = data.rank;
        ranking_[i].name = data.name;
        ranking_[i].score = data.score;
    }


    // メンバ初期化
    position_ = kBasePosition;
    player_.rank = kRegisteredNum + 1U;
    player_.name = kPlayerName;
    player_.score = 0U;

    displacement_ = static_cast<int>(kRegisteredNum) * -kLineHeight;
    disp_frame_ = displacement_ / kDispTimeMSPF;

    return true;
}

/*===========================================================================*/
// 終了処理
void RankingInEndless::destroy()
{
    if( created_ == false ) { return; }
    created_ = false;


    // タスクの登録解除
    TaskManager::getInstance()->unregisterObject(this);


    // テクスチャ開放
    TextureLoder* kTextureLoader = TextureLoder::getInstance();
    kTextureLoader->release(texture_text_);
    kTextureLoader->release(texture_number_forplayer_);
    kTextureLoader->release(texture_number_);
    kTextureLoader->release(texture_bar_);
    kTextureLoader->release(texture_);
}

/*===========================================================================*/
// 更新処理
void RankingInEndless::update()
{
    // 順位の更新
    int up = 0;
    for( int i = player_.rank - 2; i >= 0; --i )
    {
        if( ranking_[i].score <= player_.score )
        {
            ++up;
        }
        else { break; }
    }


    if ( up > 0 )
    {
        // 超した人の順位を下げる
        for( int i = 0; i < up; ++i )
        {
            ++ranking_[player_.rank - 2 - i].rank;
        }
        // 自分の順位を上げる
        player_.rank -= up;


        displacement_ += up * kLineHeight;
        disp_frame_ = displacement_ / kDispTimeMSPF;
    }


    // ランキングの移動
    position_.y += disp_frame_;
    if( (std::abs(displacement_) - std::abs(disp_frame_)) <= 0.0F )
    {
        position_.y += displacement_ - disp_frame_;
        displacement_ = disp_frame_ = 0.0F;
    }
    else
    {
        displacement_ -= disp_frame_;
    }
}

/*===========================================================================*/
// 描画処理
void RankingInEndless::draw()
{
    Vector2 draw_position;

    // プレイヤーのデータを描画
    draw_position = kBasePosition;
    drawData( player_, draw_position,
              &kTrimming[kPlayerBar], 0.8F );


    // その他プレイヤーのデータを描画
    draw_position = position_;
    draw_position.y -= 10.0F;
    for( int i = 0; i < kRegisteredNum; ++i, draw_position.y += kLineHeight )
    {
        if( draw_position.y < 0.0F ) { continue; }
        if( draw_position.y > kWindowHeight )  { break; }
        if( i + 1 == player_.rank ) { draw_position.y += kLineHeight + 20.0F; }

        drawData( ranking_[i], draw_position,
                  &kTrimming[kNormalBar], 0.8F );
    }


    // 影を描画
	Sprite::getInstance()->draw( texture_ , Vector2::Zero , nullptr , 1.0F , 0.95F );
}

/*===========================================================================*/
// ランキングデータの描画
// Data        : ランキングデータ
// Position    : 描画位置( 左端 )
// TrimmingBar : 下線切り取り範囲
// Alpha       : アルファ値
void RankingInEndless::drawData(
    const Data& Data,
    Vector2 Position,
    const RECT* const TrimminaBar,
    const float Alpha)
{
    // ランク
    if( Data.rank == player_.rank)
    {   
        // プレイヤーのランクは大きめのテクスチャ
        if( Data.rank != (kRegisteredNum + 1U ) )
        {
            Vector2 position_for_playerrank = Position;
            position_for_playerrank.x += kDeltaPositionPlayerRank.x;
            position_for_playerrank.y -= kDeltaPositionPlayerRank.y;
            Text::drawNumber( Data.rank,
                texture_number_forplayer_, position_for_playerrank,
                kPlayerRankWidth, kPlayerRankHeight, 1U, Alpha);
        }
    }
    else
    {
        Text::drawNumber( Data.rank,
            texture_number_, Position, kNumberWidth, kTextHeight, 1U, Alpha );
    }

    // 名前
    Position.x += kIntervalRankToName;
    Text::drawString( Data.name,
        texture_text_, Position, kCharacterWidth, kTextHeight, Alpha);

    // スコア
    Position.x = kDrawPositionXScore;
    Text::drawNumber( Data.score,
        texture_number_, Position, kNumberWidth, kTextHeight, kScoreDigits, Alpha);

    // 下線
    Position.x = kBarPositionX;
    Position.y += kIntervalNameToBar;
    Sprite::getInstance()->draw( texture_bar_, Position, TrimminaBar, Alpha, 0.5F );
}