
#include "sercher.h"

#include "task_manager.h"
#include "space.h"
#include "star.h"

/*===========================================================================*/
Sercher::Sercher( const Vector2& Origin, const float Radius,
                  const ObjectID Target ) :
    range_(Origin, Radius),
    target_(Target)
{

}

Sercher::~Sercher()
{
    destroy();
}

/*===========================================================================*/
// ‰Šú‰»ˆ—
bool Sercher::init( const Vector2& Origin, const float Radius, 
                    const ObjectID Target )
{
    TaskManager::getInstance()->registerTask(
        this, TaskUpdate::kSercherUpdate);
    Space::getInstance()->registration(this, Origin, Radius);


    setRange(Origin, Radius);
    target_ = Target;


    return true;
}

/*===========================================================================*/
// I—¹ˆ—
void Sercher::destroy()
{

    Space::getInstance()->unregistration( this );
    TaskManager::getInstance()->unregisterObject( this );
}

/*===========================================================================*/
// XVˆ—
void Sercher::update()
{
    Space::getInstance()->registration( this, getOrigin(), getRadius() );
}

/*===========================================================================*/
// Õ“Ëˆ—
void Sercher::collision( ObjectBase* const Object )
{
    if( Object->getID() == target_ )
    {
        collision_list_.push_back( Object );
    }
}
