#include "star.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "calc.h"

//衝突対象クラス
#include "player.h"

//定数
const int kStarMin = 60;
const int kStarDifference = 30;
const float kMinSpin = 1.0F;
const float kMaxSpin[ 3 ] =
{
	3.0F,
	6.0F,
	9.0F
};
const int kParticleTime = 3;
const float kFallSpeed = 2.0F;
const Vector3 kStarInformation[ 3 ] = {
	Vector3( 0.F,0.F,150.0F ),
	Vector3( 150.0F,0.0F,226.0F ),
	Vector3( 150.0F + 226.0F,0.0F,300.F )
};

const float kStarColorThreshold[ 2 ] =
{
	0.1F,
	0.6F
};

Star::Star()
{
	for( float& itr : angle_ )
		itr = 0;

	fall_ = 0;
	spin_ = 0;
	turn_ = 1;
	rate_ = 0;
	size_ = 0;
}

Star::~Star()
{
	TextureLoder::getInstance()->release( texture_ );
}

bool Star::init( const Vector2 & Position , const float Angle  , const float Spin , const float Rate , const float Size )
{
	TaskManager::getInstance()->registerTask( this , TaskUpdate::kStarUpdate );
	TaskManager::getInstance()->registerTask( this , TaskDraw::kParticle );

	texture_ = TextureLoder::getInstance()->load( L"Texture/star.png" );
	overlay_texture_ = TextureLoder::getInstance()->load( L"Texture/star1.png" );
	if( texture_ == nullptr || overlay_texture_ == nullptr )
		return false;

	s_particle_container_ = std::make_unique<FreeFallParticleContainer>();

	position_ = Position;
	angle_[ 0 ] = Angle;
	spin_ = Spin;
	rate_ = Rate;
	size_ = Size;

	if( Rate < kStarColorThreshold[ 0 ] )
		id_ = NameSpaceParticle::ParticleID::kCyan;
	else if( Rate < kStarColorThreshold[ 1 ] )
		id_ = NameSpaceParticle::ParticleID::kYellow;
	else
		id_ = NameSpaceParticle::ParticleID::kMagenta;

	particle_time_ = 0;
	create_point_ = 0;
	magnification_ = 1.0F;

	setAngle();

	return true;
}

void Star::destroy()
{
	s_particle_container_.get()->destroy();
	TaskManager::getInstance()->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

void Star::update()
{
	s_particle_container_.get()->update();

	position_.y += (fall_*magnification_);
	angle_[ 0 ] += (spin_ * magnification_ );
	setAngle();
}

void Star::draw()
{
	RECT trim;
	trim.left = static_cast< long >( kStarInformation[ ( static_cast< int >( size_ ) - kStarMin ) / kStarDifference ].x );
	trim.top = static_cast< long >( kStarInformation[ ( static_cast< int >( size_ ) - kStarMin ) / kStarDifference ].y ) +
		id_ * static_cast< long >( kStarInformation[ ( static_cast< int >( size_ ) - kStarMin ) / kStarDifference ].z );
	trim.right = trim.left + static_cast< long >( kStarInformation[ ( static_cast< int >( size_ ) - kStarMin ) / kStarDifference ].z );
	trim.bottom = trim.top + static_cast< long >( kStarInformation[ ( static_cast< int >( size_ ) - kStarMin ) / kStarDifference ].z );

	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	Sprite::getInstance()->draw( overlay_texture_ , position_ , &trim , 1.0F , 1.0F , Vector2( 1.0F , 1.0F ) , -( angle_[ 0 ] - 90.0F ) , Vector2( kStarInformation[ ( static_cast< int >( size_ ) - kStarMin ) / kStarDifference ].z / 2.0F , kStarInformation[ ( static_cast< int >( size_ ) - kStarMin ) / kStarDifference ].z / 2.0F ) );
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();
	Sprite::getInstance()->draw( texture_ , position_ , &trim , 1.0F , 0.9F , Vector2( 1.0F , 1.0F ) , -( angle_[ 0 ] - 90.0F ) , Vector2( kStarInformation[ ( static_cast< int >( size_ ) - kStarMin ) / kStarDifference ].z / 2.0F , kStarInformation[ ( static_cast< int >( size_ ) - kStarMin ) / kStarDifference ].z / 2.0F ) );
}

bool Star::isAlive()
{
	//画面外で死亡
	if( position_.y > 1200.0F )
		return false;

	return true;
}

void Star::setFall()
{
	fall_ = kFallSpeed;
}

void Star::collision(Player* P)
{
	float old_angle = std::atan2( -( P->getMove()->start.y - position_.y ) , P->getMove()->start.x - position_.x );
	float new_angle = std::atan2( -( P->getMove()->end.y - position_.y ) , P->getMove()->end.x - position_.x );

	if( old_angle < 0.0F ) old_angle += XM_2PI;
	if( new_angle < 0.0F ) new_angle += XM_2PI;

	if( new_angle - old_angle > XM_PI )
		new_angle += XM_2PI;
	turn_ = static_cast< int >( std::copysign( 1.0F , new_angle - old_angle ) );

	//中心からの割合
	float per = (( P->getPosition() - position_ ).Length() / size_);
	//プレイヤーの移動量を取り出す
	float p_movement = ( P->getMove()->end - P->getMove()->start ).Length();

	spin_ += turn_ * rate_ * per * p_movement;
	if( std::abs( spin_ ) < kMinSpin )
		spin_ = static_cast< float >( std::copysign( kMinSpin , spin_ ) );
	else if( std::abs( spin_ ) > kMaxSpin[ id_ ] )
		spin_ = static_cast< float >( std::copysign( kMaxSpin[ id_ ] , spin_ ) );

	particle_time_ = 0;
}

void Star::addFreeFallParticle()
{
	if( ++particle_time_ >= kParticleTime )
	{
		if( fall_ >= 1.0F )
		{
			s_particle_container_.get()->addParticle( myshape_[ create_point_++ ].start , id_ );
			particle_time_ = 0;
			if( create_point_ >= kStarLineNum )
				create_point_ = 0;
		}
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
		myshape_[ i ].end = position_ + Vector2( std::cos( XMConvertToRadians( angle_[ i ] ) ) , -std::sin( XMConvertToRadians( angle_[ i ] ) ) ) * size_;
		//線分の終点設定
		myshape_[ i ].start = position_ + Vector2( std::cos( XMConvertToRadians( angle_[ i < kStarLineNum - 1 ? i + 1 : 0 ] ) ) , -std::sin( XMConvertToRadians( angle_[ i < kStarLineNum - 1 ? i + 1 : 0 ] ) ) ) * size_;
	}
}
