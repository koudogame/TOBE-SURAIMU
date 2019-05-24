
// 板場 温樹

#include "ranking_in_endless.h"

#include "textureLoder.h"
#include "sprite.h"

#include "task_manager.h"
#include "text.h"


// ランク : 1 ~ 101

/*===========================================================================*/
constexpr long kTextWidth = 12L;
constexpr long kTextHeight = 16L;
constexpr char kPlayerName[] = "YOU";

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

    texture_ = TextureLoder::getInstance()->load(L"Texture/text.png");
    if( texture_ == nullptr ) { return false; }

    // タスクの登録
    TaskManager::getInstance()->registerTask(this, TaskUpdate::kRankingUpdate);
    TaskManager::getInstance()->registerTask(this, TaskDraw::kDraw);

    // ランキング情報を取得
    Ranking* kRanking = Ranking::getInstance();
    for( int i = 0; i < kRegisteredNum; ++i)
    {
        ranking_[i] = kRanking->getData( i + 1U );
    }

    // メンバ初期化
    position_.x = 1000.0F;
    position_.y = 0.0F;
    player_.rank = 101ULL;
    player_.name = kPlayerName;
    player_.score = 0U;

    return true;
}

/*===========================================================================*/
// 終了処理
void RankingInEndless::destroy()
{
    // タスクの登録解除
    TaskManager::getInstance()->unregisterObject(this);


    // テクスチャ開放
    TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
void RankingInEndless::update()
{
    // 順位の更新
    for(; player_.rank >= 0U; --player_.rank )
    {
        if( ranking_[player_.rank - 2].score > player_.score ) { break; }
    }
}

/*===========================================================================*/
// 描画処理
void RankingInEndless::draw()
{
    Vector2 draw_position = position_;

    for( int i = 0; i < kRegisteredNum; ++i )
    {
        Text::drawNumber( 
            ranking_[i].rank,
            texture_, draw_position, kTextWidth, kTextHeight
        );
        draw_position.x += kTextWidth * 2L;
        Text::drawString(
            ranking_[i].name,
            texture_, draw_position, kTextWidth, kTextHeight
        );
        draw_position.x = getWindowWidth<float>();
        Text::drawNumber( 
            ranking_[i].score,
            texture_, draw_position, kTextWidth, kTextHeight
        );

        draw_position.x = 1000.0F;
        draw_position.y += kTextHeight;
    }
}
