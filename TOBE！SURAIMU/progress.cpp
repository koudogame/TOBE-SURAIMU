
// 板場

#include "progress.h"

#include "task_manager.h"
#include "textureLoder.h"
#include "sprite.h"
#include "player.h"
#include "fail_wall.h"

static constexpr float kTextureStageHeight = 576.0F;
static constexpr float kStageNum = 3;
static constexpr float kTextureStageHeightSum = kTextureStageHeight * kStageNum;

static constexpr Vector2 kStageInitPosition { 250.0F, -1080.0F, };
static constexpr RECT    kStageTrimming     { 0L,  0L, 70L, 1728L };
static constexpr float   kStageDrawDepth    = 10.0F;
static constexpr float   kBasePositionY     = kStageInitPosition.y + (kTextureStageHeight * 3);

/*===========================================================================*/
Progress::Progress()
{

}

Progress::~Progress()
{
    destroy();
}


/*===========================================================================*/
bool Progress::init( const float StageHeight,
                     Player* const PPlayer,
                     FailWall* const PFailWall )
{
    // 引数をメンバにセット
    stage_height_ = StageHeight;

    // テクスチャ読み込み
    texture_ = TextureLoder::getInstance()->load( L"Texture/progress.png" );
    if( texture_ == nullptr ) { return false; }

    // その他メンバ
    stage_position_ = kStageInitPosition;
    scale_ = kTextureStageHeight / stage_height_;
    offset_ = 0.0F;




    // タスクを登録
    TaskManager* const kTaskManager = TaskManager::getInstance();
    kTaskManager->registerTask( this, TaskUpdate::kRanking );
    kTaskManager->registerTask( this, TaskDraw::kObject );

    return true;
}

void Progress::destroy()
{
    // タスクの登録を解除
    TaskManager::getInstance()->unregisterObject( this );

    // テクスチャ解放
    if( texture_ != nullptr )
    {
        TextureLoder::getInstance()->release(texture_);
        texture_ = nullptr;
    }
}

void Progress::update()
{
    
}

void Progress::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    // ステージ描画
    kSprite->reserveDraw(
        texture_,
        stage_position_,
        kStageTrimming,
        1.0F, // alpha
        kStageDrawDepth
    );
        // 上が空いたら埋める
    if( stage_position_.y > 0 )
    {
        Vector2 draw_position = stage_position_;
        draw_position.y -= kTextureStageHeightSum;
        kSprite->reserveDraw(
            texture_,
            draw_position,
            kStageTrimming,
            1.0F, // alpha
            kStageDrawDepth
        );
    }

    // プレイヤー描画

    // 壁描画

}


/*===========================================================================*/
void Progress::setMove( const float Dist )
{
    // TODO : スクロール( プレイヤーの移動 )
    // ポストイットに式かいてある
}

void Progress::changeStage()
{
    // ステージ変更を知らせる
    stage_position_.y  += kTextureStageHeight;
    if( stage_position_.y > kTextureStageHeightSum )
    {
        stage_position_ = kStageInitPosition;
    }
}
