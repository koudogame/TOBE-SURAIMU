
// 板場 温樹

#include "back_object.h"

#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"


/*===========================================================================*/
constexpr float kAmountOfAlphaChange = 0.004F;  // アルファ値変化量( 1フレーム )
constexpr float kAlphaMax            = 0.8F;    // アルファ値上限
constexpr float kAlphaMin            = 0.0F;    // アルファ値下限

/*===========================================================================*/
BackObject::BackObject()
{
}

BackObject::~BackObject()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
// Trimming : 画像切り取り範囲
// ScrollX  : x座標の1フレームでのスクロール量
// ScrollY  : y座標の1フレームでのスクロール量
// Depth    : Sprite深度値
bool BackObject::init(const RECT& Trimming,
                      const float ScrollX, const float ScrollY,
                      const float Depth)
{
    destroy();
    created_ = true;


    TextureLoder* kLoader = TextureLoder::getInstance();

    texture_         = kLoader->load(L"Texture/wave_base.png");
    if( texture_     == nullptr )     { return false; }
    texture_sub_     = kLoader->load(L"Texture/wave_sub.png");
    if( texture_sub_ == nullptr )     { return false; }


    reset(Trimming, ScrollX, ScrollY, Depth);


    return true;
}

/*===========================================================================*/
// 終了処理
void BackObject::destroy()
{
    if(created_ == false) { return; }
    created_ = false;


    TaskManager::getInstance()->unregisterObject(this);


    TextureLoder* kLoader = TextureLoder::getInstance();
    kLoader->release(texture_sub_);
    kLoader->release(texture_);
}

/*===========================================================================*/
// 更新処理
void BackObject::update()
{
    position_.x += scroll_x_ * magnification_;
    position_.y += scroll_y_ * magnification_;


    // y座標が画面外( 下 )へ行ったらそこで止める
    if( position_.y > getWindowHeight<float>() )
    {
        position_.y = getWindowHeight<float>();
    }


    // x座標が画面外へ行ったら、戻す
    const long kWidth = trimming_.right - trimming_.left;
    if( position_.x < -kWidth )
    {
        position_.x += kWidth;
    }
    else if( position_.x > kWidth )
    {
        position_.x -= kWidth;
    }


    // サブエフェクトのアルファ値を変更
    if( is_add_subalpha_ )
    {
        sub_alpha_ += kAmountOfAlphaChange;
        if( sub_alpha_ >= kAlphaMax )
        {
            sub_alpha_ = kAlphaMax;
            is_add_subalpha_ = false;
        }
    }
    else
    {
        sub_alpha_ -= kAmountOfAlphaChange;
        if( sub_alpha_ <= kAlphaMin )
        {
            sub_alpha_ = kAlphaMin;
            is_add_subalpha_ = true;

        }
    }
}

/*===========================================================================*/
// 描画処理
void BackObject::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    Vector2 draw_position = position_;

    // 描画位置のx座標が画面外( 左 )になるまで移動させる
    const long kWidth = trimming_.right - trimming_.left;
    while( draw_position.x > 0.0F )
    {
        draw_position.x -= kWidth;
    }


    // シームレスに描画( 左から右へ ) *加算合成
    kSprite->end();
    kSprite->begin(kSprite->chengeMode());
    while( draw_position.x < getWindowWidth<float>() )
    {
        kSprite->draw( texture_, draw_position, &trimming_,
                       0.2F, draw_depth_ );

        kSprite->draw( texture_sub_, draw_position, &trimming_,
                       sub_alpha_, draw_depth_);

        draw_position.x += kWidth;
    }
    kSprite->end();
    kSprite->begin();
}


/*===========================================================================*/
// メンバの設定
// Trimming : 画像切り取り範囲
// ScrollX  : x座標への1フレームでのスクロール量
// ScrollY  : y座標への1フレームでのスクロール量
// Depth    : Sprite深度値
void BackObject::reset(const RECT& Trimming,
    const float ScrollX, const float ScrollY ,
    const float Depth)
{

    TaskManager* kManager = TaskManager::getInstance();
    kManager->registerTask(this, TaskUpdate::kBackground);
    kManager->registerTask(this, TaskDraw::kBackground);


    position_.x      = static_cast<float>(rand() % ( Trimming.right - Trimming.left ));
    position_.y      = (Trimming.bottom - Trimming.top) * -1.0F;
    trimming_        = Trimming;
    scroll_x_        = ScrollX;
    scroll_y_        = ScrollY;
    draw_depth_      = Depth;
    magnification_   = 1.0F;
    is_add_subalpha_ = false;
    sub_alpha_       = 1.0F;
}
