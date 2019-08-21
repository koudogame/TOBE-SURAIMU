
// 板場　温樹

#include "fail_wall.h"

#include "release.h"
#include "shape.h"
#include "task_manager.h"
#include "space.h"
#include "textureLoder.h"
#include "sprite.h"

/*===========================================================================*/
constexpr float kLimitYUp   = getWindowHeight<float>() * 0.00F;
constexpr float kLimitYDown = getWindowHeight<float>() * 1.50F;
const Line kInitPosition { 250.0F, kLimitYDown, 1030.0F, kLimitYDown };
constexpr float kStartSpeed = -0.1F;
constexpr float kMagnificationSpeed = 1.5F;
constexpr float kScrollSpeedTable[] = 
{
    0.0F,
    -0.25F,
    -0.40F,
    -0.50F
};



constexpr int kElemNum = 4;
constexpr int kFireNum = 3;
constexpr int kWallNo = 3;
constexpr float kWidth = 1024.0F;
constexpr float kWallHeight = 256.0F;
constexpr float kFireHeight = 255.0F;
const RECT kTrimming[] =
{
    { 0L,    1L, 1024L,  256L }, // fire
    { 0L,  257L, 1024L,  512L }, // fire
    { 0L,  513L, 1024L,  768L }, // fire
    { 0L,  768L, 1024L, 1024L }, // wall
};
constexpr int kScalingFrame = 3;
constexpr int kScaleSaveNum = kScalingFrame * kFireNum;
constexpr float kAmountOfScaling = 0.05F;
constexpr float kMaxScale = 1.0F;
constexpr float kMinScale = 0.70F;


/*===========================================================================*/
FailWall::FailWall()
{

}

FailWall::~FailWall()
{
    destroy();
}


/*===========================================================================*/
bool FailWall::init()
{
    destroy();

    texture_ = TextureLoder::getInstance()->load( L"Texture/fire.png" );
    if( texture_ == nullptr ) { return false; }
    shape_ = new Line( kInitPosition );

    TaskManager::getInstance()->registerTask( this, TaskUpdate::kBackground );
    TaskManager::getInstance()->registerTask( this, TaskDraw::kFailWall );

    // 空間「0」に登録
    Space::getInstance()->registration( this, { 640.0F, 360.0F }, 10.0F );


    // メンバ初期化
    frame_counter_ = 0;
    speed_ = 0.0F;
    scaling_ = kAmountOfScaling;
    scale_y_.resize( kScaleSaveNum );
    for( auto& scale : scale_y_ )
    {
        scale = 1.0F;
    }

    return true;
}

void FailWall::destroy()
{
    Space::getInstance()->unregistration( this );
    TaskManager::getInstance()->unregisterObject( this );

    safe_delete( shape_ );

    if( texture_ )
    {
    // テクスチャの開放
        TextureLoder::getInstance()->release( texture_ );
        texture_ = nullptr;
    }
}

void FailWall::update()
{
    // 上へスクロール
    shape_->start.y += speed_;
    shape_->end.y += speed_; 
    if (shape_->start.y < kLimitYUp)
    {
        // 上限を超えたら戻す
        shape_->start.y = kLimitYUp;
    }

    // 炎のスケーリング
    if( ++frame_counter_ % 2 == 0 )
    {
        // 新しい拡大率を計算
        float scale_y = *scale_y_.begin() + scaling_;
        if( scale_y > kMaxScale )
        {
            // 既定のサイズを超える
            scale_y = kMaxScale;
            scaling_ *= -1.0F;
        }
        else if( scale_y < kMinScale )
        {
            // 既定のサイズを下回る
            scale_y = kMinScale;
            scaling_ *= -1.0F;
        }

        // スケーリング履歴を追加
        scale_y_.push_front( scale_y );

        // 古いデータを弾く
        scale_y_.pop_back();
    }
}

void FailWall::draw()
{
    Common* common = Common::getInstance();
    Sprite* sprite = Sprite::getInstance();

    // 壁の描画
    Vector2 draw_position = shape_->start;
    draw_position.y += 10.0F;
    while( draw_position.y < getWindowHeight<float>() )
    {
        sprite->reserveDraw(
            texture_,
            draw_position,
            kTrimming[kWallNo],
            1.0F, 0.8F, {1.0F, 1.0F}, 0.0F, Vector2::Zero,
            common->getStates()->Additive()
        );

        draw_position.y += kWallHeight;
    }


    // 炎の描画
    for( int i = 0; i < kFireNum; ++i )
    {
        sprite->reserveDraw(
            texture_,
            { shape_->start.x, shape_->start.y + 10.0F },
            kTrimming[i],
            1.0F,
            0.8F,
            {1.0F, scale_y_[i * kScalingFrame]},
            0.0F,
            {0.0F, kFireHeight},
            common->getStates()->Additive()
        );
    }
}


/*===========================================================================*/
void FailWall::start()
{
    speed_ = kStartSpeed;
}

void FailWall::levelUp()
{
    speed_ *= kMagnificationSpeed;
}

void FailWall::setMove( float Disp )
{
    // 上への移動は倍率をかける
    if( Disp < 0 ) { Disp *= 0.75F; }

    // 移動
    shape_->start.y += Disp;

    if( shape_->start.y < kLimitYUp )
    {
        // 上限を超えたら戻す
        shape_->start.y = kLimitYUp;
    }
    else if( shape_->start.y > kLimitYDown )
    {
        // 下限を超えたら戻す
        shape_->start.y = kLimitYDown;
    }

    // 終点も合わせて移動
    shape_->end.y = shape_->start.y;
}


/*===========================================================================*/
const Vector2& FailWall::getPosition() const 
{
    return shape_->start;
}

