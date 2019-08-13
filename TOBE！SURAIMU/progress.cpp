
// 板場

#include "progress.h"

#include "task_manager.h"
#include "textureLoder.h"
#include "sprite.h"
#include "player.h"
#include "fail_wall.h"

static constexpr float kToDegrees = 180.0F / XM_PI;
static constexpr float toDegrees( const float Radians ) { return Radians * kToDegrees; }
static constexpr float kToRadians = XM_PI / 180.0F;
static constexpr float toRadians( const float Degrees ) { return Degrees * kToRadians; }
/*===========================================================================*/
static constexpr float kTextureStageHeight = 576.0F;
static constexpr float kStageNum = 3;
static constexpr float kTextureStageHeightSum = kTextureStageHeight * kStageNum;

static constexpr Vector2 kStageInitPosition { 250.0F, -1080.0F, };
static constexpr RECT    kStageTrimming     { 0L,  0L, 70L, 1728L };
static constexpr float   kStageDrawDepth    = 10.0F;
static constexpr float   kBasePositionY     = kStageInitPosition.y + (kTextureStageHeight * 3);

static constexpr float kOffsetSinMin =  0.0F;
static constexpr float kOffsetSinMax = 90.0F;
static constexpr int   kOffsetFrameNum = 8;
static constexpr float kChangeOfSin = (kOffsetSinMax - kOffsetSinMin) / kOffsetFrameNum;


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
    // ステージのオフセットがあれば行う
    if( offset_ > 0.0F )
    {
        easingOffset();
    }
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

/*===========================================================================*/
void Progress::changeStage()
{
    // オフセット開始準備
    offset_ = kTextureStageHeight;
    offset_sin_ = kOffsetSinMin;
}

void Progress::easingOffset()
{
    float last_displacement = offset_ * std::sin(toRadians(offset_sin_));

    // サインカーブ用変数の更新
    offset_sin_ += kChangeOfSin;
    if( offset_sin_ > kOffsetSinMax ) { offset_sin_ = kOffsetSinMax; }

    // ポジションの変更( 前回との差分のみ )
    float displacement = offset_ * std::sin(toRadians(offset_sin_)) - last_displacement;
    stage_position_.y += displacement;

    // 上限まで変位したらオフセットの値をリセット
    if( offset_sin_ >= kOffsetSinMax ) 
    {
        offset_ = 0.0F;
    }
}
