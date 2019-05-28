
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
    texture_ = TextureLoder::getInstance()->load(L"Texture/roop1.png");
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
    if (position_.y > getWindowHeight<float>())
    {
        position_.y = getWindowHeight<float>();
    }

    (this->*set_angle_)();
    (this->*alpha_blend_)();
}

/*===========================================================================*/
// 描画処理
void BackObject::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    Vector2 draw_position = position_;
    draw_position.x -= getWindowWidth<float>();

    while(draw_position.x < getWindowWidth<float>())
    {
        kSprite->draw(texture_, draw_position, &trimming_);

        draw_position.x += kTextureWidth;
    }


    // 描画モード加算
#if 0
    kSprite->end();
    kSprite->begin(kSprite->chengeMode());

    RECT trimming = trimming_;
    kSprite->draw(texture_, position_, &trimming, 1.0F, 0.0F,
        Vector2(1.0F,1.0F), angle_ , kAnkerPoint);


    trimming.top += static_cast<long>(kTextureHeight);
    trimming.bottom += static_cast<long>(kTextureHeight);

    kSprite->draw(texture_, position_, &trimming, alpha_red_1, 0.0F,
        Vector2(1.0F, 1.0F), angle_, kAnkerPoint);


    trimming.top += static_cast<long>(kTextureHeight);
    trimming.bottom += static_cast<long>(kTextureHeight);

    kSprite->draw(texture_, position_, &trimming, alpha_red_2, 0.0F,
        Vector2(1.0F, 1.0F), angle_, kAnkerPoint);

    kSprite->end();
    kSprite->begin();
#endif
    // !描画モード加算
}

/*===========================================================================*/
// 再設定
void BackObject::reset(const RECT& Trimming,
    const float ScrollX, const float ScrollY ,
    const float Depth)
{
    position_.x    = getWindowWidth<float>();// / 2.0F;
    position_.y    = -kTextureHeight;
    trimming_      = Trimming;
    scroll_x_      = ScrollX;
    scroll_y_      = ScrollY;
    draw_depth_    = Depth;
    magnification_ = 1.0F;
    angle_         = 0.0F;
    set_angle_     = &BackObject::addAngle;
    alpha_red_1    = 1.0F;
    alpha_red_2    = 0.0F;
    alpha_blend_   = &BackObject::addAlphaRed2;
}

/*===========================================================================*/
// 赤1のアルファを加算
void BackObject::addAlphaRed1()
{
    // 赤2のアルファ値を0まで下げる
    if( alpha_red_2 > 0.0F)
    {
        alpha_red_2 -= kAmountOfAlphaChange;
        if( alpha_red_2 < 0.0F ) { alpha_red_2 = 0.0F; }
    }
    // 赤1のアルファ値を加算
    else
    {
        alpha_red_1 += kAmountOfAlphaChange;
        if(alpha_red_1 >= 1.0F)
        {
            alpha_red_1 = 1.0F;
            alpha_blend_ = &BackObject::addAlphaRed2;
        }
    }
}
// 赤2のアルファを加算
void BackObject::addAlphaRed2()
{
    // 赤1のアルファ値を0まで下げる
    if( alpha_red_1 > 0.0F)
    {
        alpha_red_1 -= kAmountOfAlphaChange;
        if(alpha_red_1 < 0.0F) { alpha_red_1 =  0.0F; }
    }
    // 赤2のアルファ値を加算
    else
    {
        alpha_red_2 += kAmountOfAlphaChange;
        if( alpha_red_2 >= 1.0F )
        {
            alpha_red_2 = 1.0F;
            alpha_blend_ = &BackObject::addAlphaRed1;
        }
    }
}

/*===========================================================================*/
// 角度加算
void BackObject::addAngle()
{
    angle_ += 0.05F;
    if(angle_ >= 10.0F)
    {
        set_angle_ = &BackObject::defAngle;
    }
}
// 角度減算
void BackObject::defAngle()
{
    angle_ -= 0.05F;
    if( angle_ <= -10.0F)
    {
        set_angle_ = &BackObject::addAngle;
    }
}