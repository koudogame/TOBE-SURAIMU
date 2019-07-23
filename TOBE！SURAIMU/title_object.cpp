#include "title_object.h"
#include "textureLoder.h"
#include "sprite.h"

TitleObject::TitleObject()
{}


TitleObject::~TitleObject()
{}

//‰Šú‰»
bool TitleObject::init( TitleStatus* TitleObjectStatus )
{
	status_ = TitleObjectStatus;
	return true;
}

//•`‰æ
void TitleObject::draw()
{
	Sprite::getInstance()->reserveDraw( status_->texture , status_->position , status_->trim , status_->alpha , 1.0F );
}