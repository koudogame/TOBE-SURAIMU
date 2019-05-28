
// 板場 温樹

#include "ranking_in_endless.h"

#include "textureLoder.h"
#include "sprite.h"

#include "task_manager.h"
#include "text.h"


// ランク : 1 ~ 101

/*===========================================================================*/
constexpr long kTextNumberWidth = 11L;
constexpr long kTextStringWidth = 12L;
constexpr long kTextHeight = 16L;
constexpr long kLineHeight = 25L;
constexpr char kPlayerName[] = "YOU";
constexpr long kDispTimeMSPF = 250L / 16L;
constexpr float kBarCoordinateX = 960.0F;
enum { kNormalBar, kPlayerBar };
const RECT kTrimming[] = {
    { 0L,  0L, 415L, 24L },
    { 0L, 24L, 415L, 48L }
};

void drawData(const RankingInEndless::Data&,
              Vector2 Position,
              ID3D11ShaderResourceView* const TextureText,
              ID3D11ShaderResourceView* const TextureNumber,
              ID3D11ShaderResourceView* const TextureBar,
              const RECT * const TrimmingBar,
              const float Aplha = 1.0F);

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

    texture_ = TextureLoder::getInstance()->load(L"Texture/Pray_Rank_Shadow.png");
    if( texture_ == nullptr ) { return false; }
    texture_bar_ = TextureLoder::getInstance()->load(L"Texture/mini_ranking_bar.png");
    texture_number_ = TextureLoder::getInstance()->load(L"Texture/Rank_Number.png");
    if( texture_number_ == nullptr ) { return false; }
    texture_text_ = TextureLoder::getInstance()->load(L"Texture/Rank_Name.png");
    if( texture_text_ == nullptr ) { return false; }

    // タスクの登録
    TaskManager::getInstance()->registerTask(this, TaskUpdate::kRankingUpdate);
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
    position_.x = 1000.0F;
    position_.y = getWindowHeight<float>() / 2.0F;
    player_.rank = 101ULL;
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
    // タスクの登録解除
    TaskManager::getInstance()->unregisterObject(this);


    // テクスチャ開放
    TextureLoder::getInstance()->release(texture_text_);
    TextureLoder::getInstance()->release(texture_number_);
    TextureLoder::getInstance()->release(texture_bar_);
    TextureLoder::getInstance()->release(texture_);
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
    draw_position.x = 1000.0F;
    draw_position.y = getWindowHeight<float>() / 2.0F;
    drawData( player_, draw_position,
              texture_text_, texture_number_, texture_bar_,
              &kTrimming[kPlayerBar] );

    // その他プレイヤーのデータを描画
    draw_position = position_;
    for( int i = 0; i < kRegisteredNum; ++i, draw_position.y += kLineHeight )
    {
        if( draw_position.y < 0.0F ) { continue; }
        if( draw_position.y > getWindowHeight<float>() )  { break; }
        if( i + 1 == player_.rank ) { draw_position.y += kLineHeight; }

        drawData( ranking_[i], draw_position,
                  texture_text_, texture_number_, texture_bar_,
                  &kTrimming[kNormalBar], 0.8F );
    }

    // 影を描画
	Sprite::getInstance()->draw( texture_ , Vector2::Zero , nullptr , 1.0F , 1.0F );
}


/*===========================================================================*/
void drawData(
    const RankingInEndless::Data& Data,
    Vector2 Position,
    ID3D11ShaderResourceView* const Text,
    ID3D11ShaderResourceView* const Number,
    ID3D11ShaderResourceView* const Bar,
    const RECT* const TrimminaBar,
    const float Alpha)
{
    Text::drawNumber( Data.rank,
                      Number, Position, kTextNumberWidth, kTextHeight, 1U, Alpha);

    Position.x += kTextStringWidth * 2L;
    Text::drawString( Data.name,
                      Text, Position, kTextStringWidth, kTextHeight, Alpha);

    Position.x = getWindowWidth<float>() - 23.0F;
    Text::drawNumber( Data.score,
                      Number, Position, kTextNumberWidth, kTextHeight, 10U, Alpha);

    Position.x = kBarCoordinateX;
    Position.y -= 5.0F;
    Sprite::getInstance()->draw( Bar, Position, TrimminaBar, Alpha, 1.0F );
}