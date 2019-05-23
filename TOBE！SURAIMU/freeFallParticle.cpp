#include "freeFallParticle.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"

const float kMinFall = 5.0F;
const int kTextureSize = 32;
const int kRotate = 5;

FreeFallParticle::FreeFallParticle()
{}


FreeFallParticle::~FreeFallParticle()
{}

bool FreeFallParticle::init( const std::wstring& FileName , const Vector2& Posit , const float MoveAmount )
{
	texture_ = TextureLoder::getInstance()->load( FileName );
	if( texture_ == nullptr )
		return false;

	TaskManager::getInstance()->registerTask( this , TaskUpdate::kParticleUpdate );
	TaskManager::getInstance()->registerTask( this , TaskDraw::kDraw );

	position_ = Posit;
	alpha_ = 1.0F;
	move_amount_ = MoveAmount;
	turn_ = rand() % 2 ? true : false;
	rotate_ = rand() % 72;

	return true;
}

void FreeFallParticle::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
	TaskManager::getInstance()->unregisterObject( this );
}

void FreeFallParticle::update()
{
	alpha_ -= 0.01F;
	rotate_ += turn_ ? kRotate : -kRotate;


	position_.y += kMinFall + move_amount_;
}

void FreeFallParticle::draw()
{
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	Sprite::getInstance()->draw( texture_ , position_ , nullptr , alpha_ , 0.0F , Vector2( 1.0F , 1.0F ) , static_cast< float >( rotate_ ) , Vector2( kTextureSize / 2.0F , kTextureSize / 2.0F ) );
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();
}

bool FreeFallParticle::isAlive()
{
	if( alpha_ <= 0.F )
		return false;

	return true;
}
