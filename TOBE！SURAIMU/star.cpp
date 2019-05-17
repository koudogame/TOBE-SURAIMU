#include "star.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "calc.h"

//衝突対象クラス
#include "player.h"

//定数
const int kStarMin = 134;
const float kMinSpin = 0.5F;
const float kMaxSpin = 5.0F;
const int kParticleTime = 3;

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
{
	TextureLoder::getInstance()->release( texture_ );
}

bool Star::init( const Vector2 & Position , const float Angle , const float Fall , const float Spin , const float Rate , const float Size )
{
	task_manager_->registerTask( this , TaskUpdate::kStarUpdate );
	task_manager_->registerTask( this , TaskDraw::kStarDraw );

	texture_ = TextureLoder::getInstance()->load( L"Texture/☆.png" );
	if( texture_ == nullptr )
		return false;

	s_particle_container_ = std::make_unique<FreeFallParticleContainer>( task_manager_ );

	position_ = Position;
	angle_[ 0 ] = Angle;
	temp_fall_ = Fall;
	spin_ = Spin;
	rate_ = Rate;
	size_ = Size;

	particle_time_ = 0;
	create_point_ = 0;

	setAngle();

	return true;
}

void Star::destroy()
{
	s_particle_container_.get()->destroy();
	task_manager_->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

void Star::update()
{
	s_particle_container_.get()->update();

	position_.y += (fall_*magnification_);
	angle_[ 0 ] += (spin_*magnification_);
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

bool Star::isAlive()
{
	//画面外で死亡
	if( position_.y > 1200.0F )
		return false;

	return true;
}

void Star::setMove( const float Over )
{
	position_.y += Over;
	s_particle_container_.get()->setMove( Over );
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
	else if( std::abs( spin_ ) > kMaxSpin )
		spin_ = kMaxSpin * turn_;

	particle_time_ = 0;
}

void Star::addFreeFallParticle()
{
	if( ++particle_time_ >= kParticleTime )
	{
		s_particle_container_.get()->addParticle( L"Texture/パーティクル☆.png" , myshape_[ create_point_++ ].start , (fall_*magnification_) );
		particle_time_ = 0;
		if( create_point_ >= kStarLineNum )
			create_point_ = 0;
	}
}

void Star::setAngle()
{
	//他の角度の指定
	for( int i = 0; i < kStarLineNum; i++ )
		angle_[ i ] = angle_[ 0 ] + 72 * ( i * 2 );

	for( int i = 0; i < kStarLineNum; i++ )
	{
		//線分の始点設定
		myshape_[ i ].end = position_ + Vector2( cos( XMConvertToRadians( angle_[ i ] ) ) , -sin( XMConvertToRadians( angle_[ i ] ) ) ) * size_;
		//線分の終点設定
		myshape_[ i ].start = position_ + Vector2( cos( XMConvertToRadians( angle_[ i < kStarLineNum - 1 ? i + 1 : 0 ] ) ) , -sin( XMConvertToRadians( angle_[ i < kStarLineNum - 1 ? i + 1 : 0 ] ) ) ) * size_;
	}
}
