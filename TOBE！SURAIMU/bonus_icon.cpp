
#include "bonus_icon.h"

#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"

/*===========================================================================*/
constexpr long kWidth = 80L;
constexpr long kHeight = 80L;
constexpr int kAnimationFlipFrame = 16;


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
    destroy();


    // テクスチャ読み込み
    texture_ = TextureLoder::getInstance()->load( TextureFileName );
    if (texture_ == nullptr) { return false; }

    // タスク登録
    TaskManager* const kTaskManager = TaskManager::getInstance();
    kTaskManager->registerTask(this, TaskUpdate::kBonusIcon);
    kTaskManager->registerTask(this, TaskDraw::kObject);

    // メンバ初期化
    position_ = Position;
    anim_frame_ = 0;
    anim_count_ = 0;


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
}

/*===========================================================================*/
// 描画処理
void BonusIcon::draw()
{
    RECT trimming { 0L, 0L, kWidth, kHeight };

    Sprite::getInstance()->draw(
        texture_,
        position_, 
        &trimming);
}


/*===========================================================================*/
// アニメーション
void BonusIcon::animation()
{

}
