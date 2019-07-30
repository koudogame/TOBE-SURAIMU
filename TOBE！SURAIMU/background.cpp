
#include "background.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"


constexpr float kWindowHeight = getWindowHeight<float>();

enum { kBack = 0, kMiddle, kFront };
constexpr int kLayerNum = 3;

constexpr float kScrollSpeed = 2.0F;
constexpr float kScrollMagnification[kLayerNum]
{
    0.25F, //  奥
    0.4F,  //  ↓
    0.1F,  // 手前
};

constexpr float kWidth  = 900.0F;
constexpr float kHeight = 900.0F;
template <typename T>
constexpr T getWidth()  { return static_cast<T>(kWidth);  }
template <typename T>
constexpr T getHeight() { return static_cast<T>(kHeight); } 

constexpr Vector2 kInitPosition 
{ 
    getWindowWidth<float>() / 2.0F - kWidth / 2.0F, // x( 画面中央 )
    -kHeight                                        // y( 画面外 )
};


constexpr float kDrawOffsetY = 600.0F;
constexpr float kDifSizeOffsetY = kHeight - kDrawOffsetY;
constexpr float kDrawDepth[kLayerNum]
{
    0.4F,   //  奥
    0.5F,   //  ↓
    0.6F,   // 手前
};
constexpr RECT kTrimmingStart  { 0L, 0L, getWidth<long>(), getHeight<long>() };
enum BackColor
{
    kPurple,
    kBlue,
    kRed,
};
constexpr BackColor kColorIDMin = kPurple;
constexpr BackColor kColorIDMax = kRed;


/*===========================================================================*/
Background::Background()
{

}

Background::~Background()
{
    destroy();
}


/*===========================================================================*/
bool Background::init()
{
    // テクスチャ読み込み
    if( texture_ == nullptr )
    {
        texture_ = TextureLoder::getInstance()->load( L"Texture/background.png" );
        if( texture_ == nullptr ) { return false; }
    }

    // タスク登録
    TaskManager* task_manager = TaskManager::getInstance();
    task_manager->registerTask( this, TaskUpdate::kBackground );
    task_manager->registerTask( this, TaskDraw::kBackground );


    // メンバ初期化
    if( position_ == nullptr )
    {
        position_ = new Vector2[kLayerNum];
    }
    for( int i = 0; i < kLayerNum; ++i )
    {
        position_[i] = kInitPosition;
    }

    color_ = 0;
    offset_y_ = kScrollSpeed;


    return true;
}

void Background::destroy()
{
    safe_delete_array( position_ );   

    // タスク解除
    TaskManager::getInstance()->unregisterObject( this );

    if( texture_ )
    {
    // テクスチャ開放
        TextureLoder::getInstance()->release( texture_ );
        texture_ = nullptr;
    }
}

void Background::update()
{
    // スクロール
    for( int i = 0; i < kLayerNum; ++i )
    {
        position_[i].y += offset_y_ * kScrollMagnification[i];

        // 常にきちんと重なるように位置をループ
        if( position_[i].y > -kDifSizeOffsetY )
        {
            position_[i].y -= kDrawOffsetY;
        }
    }

    offset_y_ = kScrollSpeed;
}

void Background::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    Vector2 draw_position;
    RECT    trimming = kTrimmingStart;
    // 切り取り範囲をカラーに合わせる( カラーの変化は左右 )
    trimming.left += getWidth<long>() * color_;
    trimming.right = trimming.left + getWidth<long>();

    // 各レイヤーをシームレスに描画
    for( int i = 0; i < kLayerNum ; ++i )
    {
        for( draw_position = position_[i];
             draw_position.y < kWindowHeight;
             draw_position.y += kDrawOffsetY )
        {
            kSprite->reserveDraw(
                texture_,
                draw_position,
                trimming,
                1.0F,
                kDrawDepth[i],
                {1.0F, 1.0F},
                0.0F,
                Vector2::Zero,
                Common::getInstance()->getStates()->Additive()
            );
        }


        // 切り取り範囲を次のレイヤーに( レイヤーの変化は上下 )
        trimming.top += getHeight<long>();
        trimming.bottom += getHeight<long>();
    }
}


/*===========================================================================*/
void Background::changeColor()
{
    color_ != kColorIDMax ? ++color_ : color_ = kColorIDMin;
}
