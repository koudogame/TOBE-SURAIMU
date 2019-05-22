
// ”Âê ‰·Ž÷

#include "background_container.h"


/*===========================================================================*/
BackgroundContainer::BackgroundContainer(TaskManager * const TaskManager) :
    ObjectContainerBase(TaskManager)
{
}

BackgroundContainer::~BackgroundContainer()
{
}

/*===========================================================================*/
// ”wŒi‚Ì’Ç‰Á
bool BackgroundContainer::addBackground(const RECT& Trimming,
                                        const float Scroll,
                                        const float DrawDepth)
{
    Background* background = getFreeObjAndInsert();

    if( background->init(
        Trimming,
        Scroll,
        DrawDepth
    ) == false)
    {
        return false;
    }


    return true;
}

/*===========================================================================*/
// yÀ•W‚ÌˆÚ“®( ƒXƒNƒ[ƒ‹ )
void BackgroundContainer::setMove(const float Move)
{
    for (auto& elem : active_list_)
    {
        elem->setMove(Move);
    }
}
