
// 板場 温樹

#include "back_object.h"

#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"


/*===========================================================================*/
constexpr float kTextureWidth = 2048.0F;
constexpr float kTextureHeight = 1024.0F;
constexpr float kAmountOfAlphaChange = 0.01F;
constexpr Vector2 kAnkerPoint(kTextureWidth / 2.0F, kTextureHeight / 2.0F);

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
bool BackObject::init(const RECT& Trimming,
                      const float ScrollX, const float ScrollY,
                      const float Depth)
{
    destroy();
    created_ = true;

    // 生成処理
    texture_ = TextureLoder::getInstance()->load(L"Texture/wave.png");
    if (texture_ == nullptr) { return false; }

    TaskManager::getInstance()->registerTask(this, TaskUpdate::kBackgroundUpdate);
    TaskManager::getInstance()->registerTask(this, TaskDraw::kBackground);


    // 初期化
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
    TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
void BackObject::update()
{
    position_.x += scroll_x_ * magnification_;
    position_.y += scroll_y_ * magnification_;

    // 画面外へ行ったらそこで止める
    if( position_.y > getWindowHeight<float>() )
    {
        position_.y = getWindowHeight<float>();
    }
    // 右端が画面左端を超えたら、
    const long kWidth = trimming_.right - trimming_.left;
    if( position_.x + kWidth < 0.0F )
    {
        position_.x -= kWidth;
    }
}

/*===========================================================================*/
// 描画処理
void BackObject::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    Vector2 draw_position = position_;
    draw_position.x -= getWindowWidth<float>();

    // シームレスに描画
    while(draw_position.x < getWindowWidth<float>())
    {
        kSprite->draw(texture_, draw_position, &trimming_);

        draw_position.x += kTextureWidth;
    }
}

/*===========================================================================*/
// 再設定
void BackObject::reset(const RECT& Trimming,
    const float ScrollX, const float ScrollY ,
    const float Depth)
{
    position_.x    = 0.0F;
    position_.y    = (Trimming.bottom - Trimming.top) * -1.0F;
    trimming_      = Trimming;
    scroll_x_      = ScrollX;
    scroll_y_      = ScrollY;
    draw_depth_    = Depth;
    magnification_ = 1.0F;
    is_add_red_1_  = true;
    alpha_red_1_   = 1.0F;
    alpha_red_2_   = 0.0F;
}