
// 板場 温樹

#include "back_object.h"

#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"


/*===========================================================================*/
constexpr float kTextureWidth = 1280.0F;
constexpr float kTextureHeight = 720.0F;
constexpr float kAmountOfAlphaChange = 0.01F;

/*===========================================================================*/
BackObject::BackObject(TaskManager* const TaskManager) :
    ObjectBase(TaskManager)
{
}

BackObject::~BackObject()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool BackObject::init(const RECT& Trimming, const float Scroll)
{
    destroy();
    created_ = true;

    // 生成処理
    texture_ = TextureLoder::getInstance()->load(L"Texture/background.png");
    if (texture_ == nullptr) { return false; }

    task_manager_->registerTask(this, TaskUpdate::kBackgroundUpdate);
    task_manager_->registerTask(this, TaskDraw::kBackgroundDraw);


    // 初期化
    position_.x = getWindowWidth<float>() / 2.0F - kTextureWidth / 2.0F;
    position_.y = -kTextureHeight;
    trimming_ = Trimming;
    scroll_ = Scroll;
    switch(rand() % 2)
    {
    case 0:
        alpha_red_1 = 1.0F;
        alpha_red_2 = 0.0F;
        alpha_blend_ = &BackObject::addAlphaRed2;
        break;
    case 1:
        alpha_red_1 = 0.0F;
        alpha_red_2 = 1.0F;
        alpha_blend_ = &BackObject::addAlphaRed1;
        break;
    }

    return true;
}

/*===========================================================================*/
// 終了処理
void BackObject::destroy()
{
    if(created_ == false) { return; }
    created_ = false;

    task_manager_->unregisterObject(this);
    TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
void BackObject::update()
{
    position_.y += scroll_ * magnification_;
    if (position_.y > getWindowHeight<float>())
    {
        position_.y = getWindowHeight<float>();
    }
    (this->*alpha_blend_)();
}

/*===========================================================================*/
// 描画処理
void BackObject::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    RECT trimming = trimming_;
    kSprite->draw(texture_, position_, &trimming);

    trimming.top += static_cast<long>(kTextureHeight);
    trimming.bottom += static_cast<long>(kTextureHeight);
    kSprite->draw(texture_, position_, &trimming, alpha_red_1);

    trimming.top += static_cast<long>(kTextureHeight);
    trimming.bottom += static_cast<long>(kTextureHeight);
    kSprite->draw(texture_, position_, &trimming, alpha_red_2);
}

/*===========================================================================*/
// 再設定
void BackObject::reset(const RECT& Trimming)
{
    trimming_ = Trimming;
    position_.y = -kTextureHeight;
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
