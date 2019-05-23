
// 板場 温樹

#include "ranking_in_endless.h"

#include "textureLoder.h"
#include "sprite.h"
#include "ranking.h"
#include "task_manager.h"
#include "text.h"
#include "numbers.h"

/*===========================================================================*/
RankingInEndless::RankingInEndless() :
    ranking_(100)
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
    for( unsigned i = 0U; i < kRegisteredNum; ++i)
    {
        data = kRanking->getData( i + 1U );
        ranking_[i] = std::make_pair(
                Text(data.first.c_str()),
                Numbers<ULONGLONG>(data.second) 
            );
    }
    player_ = std::make_pair("You", 0ULL);

    // メンバ初期化
    position_.x = 1000.0F;
    position_.y = 100.0F;
    score_ = 0ULL;
    rank = kRegisteredNum;


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

}

/*===========================================================================*/
// 描画処理
void RankingInEndless::draw()
{
    Vector2 draw_position = position_;
    for( unsigned i = rank - 10U; i < rank; ++i)
    {
        ranking_[i].first.draw( texture_, draw_position, 12L, 16L );

        draw_position.x = 1280.0F;
        ranking_[i].second.draw( texture_, draw_position, 12L, 16L );

        draw_position.x = position_.x;
        draw_position.y += 15.0F;
    }
    player_.first.draw( texture_, draw_position, 12L, 16L );
    draw_position.x = position_.x;
    player_.second.draw( texture_, draw_position, 12L, 16L );
}
