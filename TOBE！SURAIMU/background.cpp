
#include "background.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"


constexpr float kWindowHeight = getWindowHeight<float>();

enum { kFront = 0, kMiddle, kBack };
constexpr int kLayerNum = 3;

constexpr float kScrollSpeed = 1.0F;
constexpr float kScrollMagnification[kLayerNum]
{
    1.0F,   // 手前
    0.4F,   //  ↓
    0.25F,  //  奥
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
    2.0F,   // 手前
    1.0F,   //  ↓
    0.0F,   //  奥
};
constexpr RECT kTrimmingStart  { 0L, 0L, getWidth<long>(), getHeight<long>() };


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
    destroy();

    // テクスチャ読み込み
    texture_ = TextureLoder::getInstance()->load( L"Texture/background.png" );
    if( texture_ == nullptr ) { return false; }

    // タスク登録
    TaskManager* task_manager = TaskManager::getInstance();
    task_manager->registerTask( this, TaskUpdate::kBackground );
    task_manager->registerTask( this, TaskDraw::kBackground );


    // メンバ初期化
    position_ = new Vector2[kLayerNum];
    for( int i = 0; i < kLayerNum; ++i )
    {
        position_[i] = kInitPosition;
    }

    offset_y_ = kScrollSpeed;


    return true;
}

void Background::destroy()
{
    safe_delete_array( position_ );   

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
            position_[i].y -= kDifSizeOffsetY;
        }
    }
}

void Background::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    Vector2 draw_position;
    RECT    trimming = kTrimmingStart;
    // ここでレベルにあった位置に動かす

    // 各レイヤーをシームレスに描画
    for( int i = 0; i < kLayerNum; ++i )
    {
        // 切り取り範囲をレイヤーに合った位置に動かす
        trimming.top += getHeight<long>();

        for( draw_position = position_[i];
             draw_position.y + kHeight < kWindowHeight;
             draw_position.y += kDrawOffsetY )
        {

            kSprite->reserveDraw(
                texture_,
                draw_position,
                trimming,
                1.0F,
                kDrawDepth[i]
            );
        }
    }
}


/*===========================================================================*/
