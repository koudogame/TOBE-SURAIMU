#include "star.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"

//’è”
const int kStarMin = 134;

Star::Star( TaskManager * const Manager ) :
	ObjectBase( ObjectID::kStar , Manager )
{
	for( float& itr : angle_ )
		itr = 0;

	fall_ = 0;
	temp_fall_ = 0;
	spin_ = 0;
	turn_ = 1;
	rate_ = 0;
	size_ = 0;
}

Star::~Star()
{}

bool Star::init( const Vector2 & Position , const float Angle , const float Fall , const float Spin , const float Rate , const float Size )
{
	task_manager_->registerTask( this , TaskUpdate::kStarUpdate );
	task_manager_->registerTask( this , TaskDraw::kStarDraw );

	texture_ = TextureLoder::getInstance()->load( L"Texture/™.png" );
	if( texture_ == nullptr )
		return false;

	position_ = Position;
	angle_[ 0 ] = Angle;
	temp_fall_ = Fall;
	spin_ = Spin;
	rate_ = Rate;
	size_ = Size;

	setAngle();

	return true;
}

void Star::destroy()
{
	task_manager_->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

void Star::update()
{
	position_.y += fall_;
	angle_[ 0 ] += spin_;
	setAngle();
}

void Star::draw()
{
	long edge[ 4 ] = { 0, kStarMin, kStarMin * 3, kStarMin * 5 };
	RECT trim = {
		edge[ static_cast< int >( size_ ) / 50 - 1 ],
		0,
		trim.left + edge[ static_cast< int >( size_ ) / 50 ],
		trim.top + edge[ static_cast< int >( size_ ) / 50 ] };

	Sprite::getInstance()->draw( texture_ , position_ , &trim , 1.0F , 0.0F , Vector2( 1.0F , 1.0F ) , angle_[ 0 ] - 90 , Vector2( ( size_ / 50 * kStarMin ) / 2.0F , ( size_ / 50 * kStarMin ) / 2.0F ) );
}

bool Star::isLife()
{
	//‰æ–ÊŠO‚ÅŽ€–S
	if( position_.y > 1200.0F )
		return false;

	return true;
}

void Star::setAngle()
{
	//‘¼‚ÌŠp“x‚ÌŽw’è
	for( int i = 0; i < 5; i++ )
		angle_[ i ] = angle_[ 0 ] + 72 * ( i * 2 );
}
