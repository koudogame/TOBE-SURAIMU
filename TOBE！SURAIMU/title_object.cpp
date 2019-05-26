#include "title_object.h"
#include "textureLoder.h"
#include "sprite.h"

const int kFall = 10;

TitleObject::TitleObject()
{}


TitleObject::~TitleObject()
{}

bool TitleObject::init( TitleStatus* TitleObjectStatus )
{
	status_ = TitleObjectStatus;
	return true;
}

void TitleObject::update()
{
	status_->position.y += kFall;
}

void TitleObject::draw()
{
	Sprite::getInstance()->draw( status_->texture , status_->position , &status_->trim , status_->alpha );
}