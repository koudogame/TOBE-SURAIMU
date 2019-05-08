#include "commonstates.h"

bool Common::init()
{
	status_ = make_unique<CommonStates>(Direct3D::getInstance()->getDevice());
    if( !status_.get())
        //  ƒGƒ‰[
        return false;

    return true;
}