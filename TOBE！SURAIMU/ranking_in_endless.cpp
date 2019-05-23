
// 板場 温樹

#include "ranking_in_endless.h"

#include "textureLoder.h"
#include "sprite.h"
#include "ranking.h"
#include "task_manager.h"
#include "text.h"
#include "numbers.h"

/*===========================================================================*/
constexpr long kTextWidth = 11L;
constexpr long kTextHeight = 16L;
constexpr char kPlayerName[] = "YOU";

/*===========================================================================*/
RankingInEndless::RankingInEndless() :
    ranking_(kRegisteredNum),
    player_name_(kPlayerName)
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
    std::pair<std::string, ULONGLONG> data;
    Ranking* kRanking = Ranking::getInstance();
    for( int i = 0; i < kRegisteredNum; ++i)
    {
        data = kRanking->getData( i + 1U );
        ranking_[i] = std::make_pair(
                Text(data.first.c_str()), Numbers<ULONGLONG>(data.second));
    }

    // メンバ初期化
    position_.x = 1000.0F;
    position_.y = getWindowHeight<float>() - kTextHeight;
    rank_ = kRegisteredNum + 1U;
    score_ = 0ULL;

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
    for(; rank_ >= 0U; --rank_ )
    {
        if( ranking_[rank_ - 2].second.get() > score_.get() ) { break; }
    }
}

/*===========================================================================*/
// 描画処理
void RankingInEndless::draw()
{
    Vector2 draw_position = position_;

    // プレイヤーのスコアを描画
    player_name_.draw( texture_, draw_position, kTextWidth, kTextHeight );
    draw_position.x = getWindowWidth<float>();
    score_.draw( texture_, draw_position, kTextWidth, kTextHeight );

    // ランキングを描画
    for( int rank = rank_ - 2U; rank >= 0; --rank )
    {
        draw_position.x = position_.x;
        draw_position.y -= kTextHeight;
        if( draw_position.y < 0.0F ) { break; }


        ranking_[rank].first.draw( texture_, draw_position, kTextWidth, kTextHeight );
        draw_position.x = getWindowWidth<float>();
        ranking_[rank].second.draw( texture_, draw_position, kTextWidth, kTextHeight );
    }
}
