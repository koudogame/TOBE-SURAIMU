
#include "background.h"

#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"

const long kTextureSize = 1024L;

/*===========================================================================*/
Background::Background(TaskManager* const TaskManager) :
    ObjectBase(TaskManager)
{
}

Background::~Background()
{
}

/*===========================================================================*/
// 初期化処理
bool Background::init(const wchar_t* const TextureFile, const float Scroll)
{
    // テクスチャ読み込み
    texture_ = TextureLoder::getInstance()->load(TextureFile);
    if (texture_ == nullptr) { return false; }

    // タスクの登録
    task_manager_->registerTask(this, TaskUpdate::kBackgroundUpdate);
    task_manager_->registerTask(this, TaskDraw::kBackgroundDraw);

    // メンバ
    position_.x = (getWindowWidth<float>() - static_cast<float>(kTextureSize)) / 2.0F;
    position_.y = 0.0F;
    scroll_ = Scroll;

    return true;
}

/*===========================================================================*/
// 終了処理
void Background::destroy()
{
    // タスク
    task_manager_->unregisterObject(this);
   
    // テクスチャ開放
    TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
void Background::update()
{
    position_.y += scroll_;
    if (position_.y > getWindowHeight<float>())
    {
        position_.y = getWindowHeight<float>() - kTextureSize + scroll_;
    }
}

/*===========================================================================*/
// 描画処理
void Background::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    Vector2 draw_position = position_;
    while (draw_position.y + kTextureSize > 0.0F)
    {
        kSprite->draw(texture_, draw_position);

        draw_position.y -= static_cast<float>(kTextureSize);
    }
    kSprite->draw(texture_, draw_position);
}
