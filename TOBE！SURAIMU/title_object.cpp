#include "title_object.h"
#include "textureLoder.h"
#include "sprite.h"

const int kFall = 10;

TitleObject::TitleObject()
{}


TitleObject::~TitleObject()
{}

bool TitleObject::init( const Vector2 & Posit , const RECT & Triming )
{
	texture_ = TextureLoder::getInstance()->load( L"Texture/Title.png" );
	if( texture_ == nullptr )
		return false;
	position_ = Posit;
	trim_ = Triming;

	return true;
}

void TitleObject::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
}

void TitleObject::update()
{
	position_.y += kFall;
}

void TitleObject::draw()
{
	Sprite::getInstance()->draw( texture_ , position_ , &trim_ );
}

bool TitleObject::isAlive()
{
	if( position_.y > 720.0F )
		return false;
	return true;
}
