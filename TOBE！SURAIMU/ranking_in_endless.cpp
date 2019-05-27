
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

void drawData(const RankingInEndless::Data&,
              Vector2 Position,
              ID3D11ShaderResourceView* const TextureText,
              ID3D11ShaderResourceView* const TextureNumber,
              ID3D11ShaderResourceView* const TextureBar);

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
    texture_bar_ = TextureLoder::getInstance()->load(L"Texture/Pray_Rank_Bar.png");
    texture_number_ = TextureLoder::getInstance()->load(L"Texture/Rank_Number.png");
    if( texture_number_ == nullptr ) { return false; }
    texture_text_ = TextureLoder::getInstance()->load(L"Texture/Rank_Name.png");
    if( texture_text_ == nullptr ) { return false; }

    // タスクの登録
    TaskManager::getInstance()->registerTask(this, TaskUpdate::kRankingUpdate);
    TaskManager::getInstance()->registerTask(this, TaskDraw::kDraw);

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
    position_.y = (getWindowHeight<float>() / 2.0F) -
                  kLineHeight * kRegisteredNum;
    player_.rank = 101ULL;
    player_.name = kPlayerName;
    player_.score = 0U;

    displacement_ = 0.0F;
    disp_frame_ = 0.0F;

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
    if( (displacement_ -= disp_frame_) <= 0.0F )
    {
        position_.y += displacement_;
        displacement_ = disp_frame_ = 0.0F;
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
    drawData( player_, draw_position, texture_text_, texture_number_, texture_bar_);

    // その他プレイヤーのデータを描画
    draw_position = position_;
    for( int i = 0; i < kRegisteredNum; ++i, draw_position.y += kLineHeight )
    {
        if( draw_position.y < 0.0F ) { continue; }
        if( draw_position.y > getWindowHeight<float>() )  { break; }
        if( i + 1 == player_.rank ) { draw_position.y += kLineHeight; }
        
        drawData( ranking_[i], draw_position, texture_text_, texture_number_, texture_bar_);
    }

    // 影を描画
    Sprite::getInstance()->draw(texture_, Vector2::Zero);
}


/*===========================================================================*/
void drawData(
    const RankingInEndless::Data& Data,
    Vector2 Position,
    ID3D11ShaderResourceView* const Text,
    ID3D11ShaderResourceView* const Number,
    ID3D11ShaderResourceView* const Bar)
{
    Text::drawNumber( Data.rank,
                      Number, Position, kTextNumberWidth, kTextHeight );

    Position.x += kTextStringWidth * 2L;
    Text::drawString( Data.name,
                      Text, Position, kTextStringWidth, kTextHeight );

    Position.x = getWindowWidth<float>();
    Text::drawNumber( Data.score,
                      Number, Position, kTextNumberWidth, kTextHeight, 10U );

    Position.x = 850.0F;
    Position.y += 5L;
    Sprite::getInstance()->draw( Bar, Position );
}