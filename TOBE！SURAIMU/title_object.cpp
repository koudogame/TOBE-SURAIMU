#include "title_object.h"
#include "textureLoder.h"
#include "sprite.h"

//•`‰æ
void TitleStatus::draw()
{
	Sprite::getInstance()->reserveDraw( texture , position , trim , alpha , depth );
}