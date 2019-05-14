#include "star.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "calc.h"

//衝突対象クラス
#include "player.h"

//定数
const int kStarMin = 134;
const float kMinSpin = 0.F;

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

	texture_ = TextureLoder::getInstance()->load( L"Texture/☆.png" );
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
	RECT trim;
	trim.left = 0;
	for( int i = 0; i < ( size_ / 50L - 1L ); i++ )
		trim.left += ( i + 1 ) * kStarMin;
	trim.top = 0L;
	trim.right = trim.left + static_cast< long >( size_ / 50L ) * kStarMin;
	trim.bottom = trim.top + static_cast< long >( size_ / 50L ) * kStarMin;

	Sprite::getInstance()->draw( texture_ , position_ , &trim , 1.0F , 0.0F , Vector2( 1.0F , 1.0F ) , -(angle_[ 0 ] - 90.0F) , Vector2( ( size_ / 50 * kStarMin ) / 2.0F , ( size_ / 50 * kStarMin ) / 2.0F ) );
}

bool Star::isLife()
{
	//画面外で死亡
	if( position_.y > 1200.0F )
		return false;

	return true;
}

void Star::collision(Player* P)
{
	float old_angle = atan2( -( P->getMove()->start.y - position_.y ) , ( P->getMove()->start.x - position_.x ) );
	float new_angle = atan2( -( P->getMove()->end.y - position_.y ) , ( P->getMove()->end.x - position_.x ) );
	int signal = ( int ) copysign( 1.0F , new_angle - old_angle );
	turn_ = signal;

	spin_ += turn_ * rate_ * abs( Calc::cross( P->getMove()->end - P->getMove()->start , P->getShape()->position - position_ ) );
	if( std::abs( spin_ ) < kMinSpin )
		spin_ = kMinSpin * turn_;
}

void Star::setAngle()
{
	//他の角度の指定
	for( int i = 0; i < 5; i++ )
		angle_[ i ] = angle_[ 0 ] + 72 * ( i * 2 );

	for( int i = 0; i < 5; i++ )
	{
		//線分の始点設定
		myshape_[ i ].end = position_ + Vector2( cos( XMConvertToRadians( angle_[ i ] ) ) , -sin( XMConvertToRadians( angle_[ i ] ) ) ) * size_;
		//線分の終点設定
		myshape_[ i ].start = position_ + Vector2( cos( XMConvertToRadians( angle_[ i < 4 ? i + 1 : 0 ] ) ) , -sin( XMConvertToRadians( angle_[ i < 4 ? i + 1 : 0 ] ) ) ) * size_;
	}
}
