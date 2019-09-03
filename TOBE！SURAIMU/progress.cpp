
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

static constexpr float kScrollSpeed = 2.0F;
// Stage
/*===========================================================================*/
static constexpr float kTextureStageHeight = 576.0F;
static constexpr float kStageNum = 3;
static constexpr float kTextureStageHeightSum = kTextureStageHeight * kStageNum;
static constexpr Vector2 kStageInitPosition { 250.0F, -1080.0F, };
static constexpr RECT    kStageTrimming     { 0L,  0L, 83L, 1728L };
static constexpr float   kStageDrawDepth    = 10.0F;
static constexpr float   kBasePositionY     = kStageInitPosition.y + (kTextureStageHeight * 3);
static constexpr float kOffsetSinMin =  0.0F;
static constexpr float kOffsetSinMax = 90.0F;
static constexpr int   kOffsetFrameNum = 32;
static constexpr float kChangeOfSin = (kOffsetSinMax - kOffsetSinMin) / kOffsetFrameNum;
// Player
/*===========================================================================*/
static constexpr float   kTexturePlayerHeight = 83.0F;
static constexpr Vector2 kPlayerBasePosition  { 250.0F, 613.0F };
static constexpr RECT    kPlayerTrimming      { 140L, 0L, 210L, 70L };
static constexpr float   kPlayerDrawDepth     = 11.0F; 
// Wall
/*===========================================================================*/
static constexpr RECT  kWallTrimming { 70L, 0L, 140L, 720L };
static constexpr float kWallDrawDepth = 12.0F;


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
    player_       = PPlayer;
    wall_ = PFailWall;

    // テクスチャ読み込み
    texture_ = TextureLoder::getInstance()->load( L"Texture/progress.png" );
    if( texture_ == nullptr ) { return false; }

    // その他メンバ
    scale_ = kTextureStageHeight / stage_height_;
    stage_position_ = kStageInitPosition;
    offset_ = 0.0F;

    player_base_position_ = kPlayerBasePosition;
    player_displacement_ = -player_->getPosition().y;
    player_last_position_y_ = player_->getPosition().y;

    wall_last_position_y_ = wall_->getPosition().y;

    last_distance_player_wall_ = wall_last_position_y_ - player_last_position_y_;

    scroll_speed_ = 0.0F;

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


    // プレイヤーの位置更新
    float player_delta = getPlayerDelta();
    player_displacement_ += player_delta;
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
    Vector2 player_draw_position = player_base_position_;
    //                        変位の大きさ最大　　　　 プレイヤーのステージ上の位置の割合
    player_draw_position.y -= kTextureStageHeight * (player_displacement_ / stage_height_);
    kSprite->reserveDraw(
        texture_,
        player_draw_position,
        kPlayerTrimming,
        1.0F, // alpha
        kPlayerDrawDepth
    );

    // 壁描画( プレイヤーとの距離から描画位置を算出 )
    Vector2 wall_draw_position = player_draw_position;
    wall_draw_position.y += kTexturePlayerHeight * 0.5F;
    float dist = wall_->getPosition().y - player_->getPosition().y;
    wall_draw_position.y += dist * scale_;

    if( wall_draw_position.y < getWindowHeight<float>() )
    {
        kSprite->reserveDraw(
            texture_,
            wall_draw_position,
            kWallTrimming,
            1.0F, // alpha
            kWallDrawDepth
        );
    }
}


/*===========================================================================*/
void Progress::setMove( const float Dist )
{
    // スクロール( プレイヤーの移動 )
    if( player_->isJump() )
    {
        player_displacement_ += Dist;
    }
}

/*===========================================================================*/
void Progress::start()
{
    scroll_speed_ = kScrollSpeed;
}

/*===========================================================================*/
void Progress::changeStage()
{
    // オフセット開始準備
    offset_ = kTextureStageHeight;
    offset_sin_ = kOffsetSinMin;
}


/*===========================================================================*/
void Progress::easingOffset()
{
    float last_displacement = offset_ * std::sin(toRadians(offset_sin_));

    // サインカーブ用変数の更新
    offset_sin_ += kChangeOfSin;
    if( offset_sin_ > kOffsetSinMax ) { offset_sin_ = kOffsetSinMax; }

    // ポジションの変更( 前回との差分のみ )
    float displacement = offset_ * std::sin(toRadians(offset_sin_)) - last_displacement;
    stage_position_.y += displacement;
    player_base_position_.y += displacement;

    // 上限まで変位したら
    if( offset_sin_ >= kOffsetSinMax ) 
    {
        offset_ = 0.0F;
        player_base_position_ = kPlayerBasePosition;
        player_displacement_ -= stage_height_;
    }
}



// 下方向への移動が"-"
// 上方向への移動が"+"
/*===========================================================================*/
float Progress::getPlayerDelta()
{
    float curr_player_y = player_->getPosition().y;
    float curr_wall_y = wall_->getPosition().y;

    float dist_p_w = curr_wall_y - curr_player_y;

    float delta = dist_p_w - last_distance_player_wall_ - wall_->getSpeed();
    delta += scroll_speed_;

    player_last_position_y_ = curr_player_y;
    wall_last_position_y_ = curr_wall_y;
    last_distance_player_wall_ = dist_p_w;

    return delta;
}
