
// ”Âê@‰·÷

#include "fail_wall.h"

#include "release.h"
#include "shape.h"
#include "task_manager.h"
#include "space.h"
#include "textureLoder.h"
#include "sprite.h"

/*===========================================================================*/
constexpr float kLimitYUp   = getWindowHeight<float>() * 0.90F;
constexpr float kLimitYDown = 1000.0F;
constexpr float kScrollSpeed = 0.5F;
const RECT kTrimming { 320L, 1704L, 960L, 1744L };
const Line kInitPosition { 320.0F, kLimitYDown, 960.0F, kLimitYDown };


/*===========================================================================*/
FailWall::FailWall()
{

}

FailWall::~FailWall()
{
    destroy();
}


/*===========================================================================*/
bool FailWall::init()
{
    destroy();

    texture_ = TextureLoder::getInstance()->load( L"Texture/Background.png" );
    if( texture_ == nullptr ) { return false; }
    shape_ = new Line( kInitPosition );

    TaskManager::getInstance()->registerTask( this, TaskUpdate::kBackground );
    TaskManager::getInstance()->registerTask( this, TaskDraw::kBackground );

    Space::getInstance()->registration( this, { 640.0F, 360.0F }, 10.0F );


    return true;
}

void FailWall::destroy()
{
    Space::getInstance()->unregistration( this );
    TaskManager::getInstance()->unregisterObject( this );
    safe_delete( shape_ );

    if( texture_ )
    {
        TextureLoder::getInstance()->release( texture_ );
        texture_ = nullptr;
    }
}

void FailWall::update()
{
    shape_->start.y -= kScrollSpeed;
    shape_->end.y   -= kScrollSpeed;
}

void FailWall::draw()
{
    Sprite::getInstance()->draw(
        texture_,
        shape_->start,
        &kTrimming
    );
}


/*===========================================================================*/
void FailWall::setMove( const float Disp )
{
    // ˆÚ“®
    shape_->start.y += Disp;

    if( shape_->start.y < kLimitYUp )
    {
    // ãŒÀ‚ğ’´‚¦‚½‚ç–ß‚·
        shape_->start.y = kLimitYUp;
    }
    else if( shape_->start.y > kLimitYDown )
    {
    // ‰ºŒÀ‚ğ’´‚¦‚½‚ç–ß‚·
        shape_->start.y = kLimitYDown;
    }

    // I“_‚àˆÚ“®
    shape_->end.y = shape_->start.y;
}
