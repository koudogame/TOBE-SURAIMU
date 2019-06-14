
// 板場 温樹

#include "reflect_icon.h"

#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"


/*===========================================================================*/
const RECT kTrimming { 0L, 0L, 80L, 80L };


/*===========================================================================*/
ReflectIcon::ReflectIcon()
{

}

ReflectIcon::~ReflectIcon()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool ReflectIcon::init( const Vector2& Position )
{
    // テクスチャ読み込み
    if( texture_ == nullptr )
    {
       texture_ = TextureLoder::getInstance()->load(L"Texture/bonus_icon.png");
       if( texture_ == nullptr ) { return false; }
    }

    // タスク登録
    TaskManager* const kTaskManager = TaskManager::getInstance();
    kTaskManager->registerTask( this, TaskUpdate::kBonusIcon );
    kTaskManager->registerTask( this, TaskDraw::kObject );

    // メンバ初期化
    position_ = Position;


    return true;
}

/*===========================================================================*/
// 終了処理
void ReflectIcon::destroy()
{
    // タスク登録解除
    TaskManager::getInstance()->unregisterObject( this );

    // テクスチャ開放
    if( texture_ != nullptr )
    {
        TextureLoder::getInstance()->release( texture_ );
        texture_ = nullptr;
    }
}

/*===========================================================================*/
// 更新処理
void ReflectIcon::update()
{
    if( position_.y > getWindowHeight<float>() )
    {
        TaskManager::getInstance()->unregisterObject( this );
    }
}

/*===========================================================================*/
// 描画処理
void ReflectIcon::draw()
{
    Sprite::getInstance()->draw(
        texture_,
        position_,
        &kTrimming );
}
