
#include "bonus_icon.h"

#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"

/*===========================================================================*/
constexpr int kAnimationFlipFrame =16;


/*===========================================================================*/
BonusIcon::BonusIcon()
{

}

BonusIcon::~BonusIcon()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool BonusIcon::init( const wchar_t* const TextureFileName,
                      const Vector2&       Position )
{
    // テクスチャ読み込み
    if (texture_ == nullptr)
    {
        texture_ = TextureLoder::getInstance()->load( TextureFileName );
        if (texture_ == nullptr) { return false; }
    }

    // タスク登録
    TaskManager* const kTaskManager = TaskManager::getInstance();
    kTaskManager->registerTask(this, TaskUpdate::kBonusIcon);
    kTaskManager->registerTask(this, TaskDraw::kObject);

    // メンバ初期化
    position_ = Position;


    return true;
}

/*===========================================================================*/
// 終了処理
void BonusIcon::destroy()
{
    // タスク登録解除
    TaskManager::getInstance()->unregisterObject(this);

    // テクスチャ開放
    if (texture_ != nullptr)
    {
        TextureLoder::getInstance()->release(texture_);
        texture_ = nullptr;
    }
}

/*===========================================================================*/
// 更新処理
void BonusIcon::update()
{
    // アニメーション処理
    animation();


    // 画面外へ行ったら、タスク登録解除
    if (position_.y > getWindowHeight<float>())
    {
        TaskManager::getInstance()->unregisterObject(this);
    }
}

/*===========================================================================*/
// 描画処理
void BonusIcon::draw()
{
    Sprite::getInstance()->draw(
        texture_,
        position_ );
}


/*===========================================================================*/
// アニメーション
void BonusIcon::animation()
{

}
