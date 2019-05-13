#include "player.h"
#include "task_manager.h"
#include "textureLoder.h"
#include "sprite.h"
#include "pad.h"
#include "key.h"
#include "calc.h"

//判定対象クラス
#include "star.h"
#include "wall.h"

const int kPlayerSize = 30;


Player::Player( TaskManager* const Manager ) :
	ObjectBase( ObjectID::kPlayer , Manager )
{
	kGround.start = Vector2( 0.0F , getWindowHeight<float>() );
	kGround.end = Vector2( getWindowWidth<float>() , getWindowHeight<float>() );
}


Player::~Player()
{
	TextureLoder::getInstance()->release( texture_ );
}

bool Player::init( const Vector2 & Posit , const float Jump , const float Decay , const float Gravity , const float Speed , const float Boost )
{
	task_manager_->registerTask( this , TaskUpdate::kPlayerUpdate );
	task_manager_->registerTask( this , TaskDraw::kPlayerDraw );
	myshape_ = Circle( Posit , 7.5F );
	kJumpPower = Jump;
	kDecay = Decay;
	kGravity = Gravity;
	kSpeed = Speed;
	kBoostPower = Boost;
	ground_ = kGround;
	texture_ = TextureLoder::getInstance()->load( L"Texture/motion dummy.png" );
	if( texture_ == nullptr )
		return false;

	return true;
}

void Player::destroy()
{
	task_manager_->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

void Player::update()
{
	move_vector_.start = myshape_.position;
	movement_ = Vector2::Zero;

	//重力をかける
	gravity();

	//入力時処理
	input();

	//ジャンプ力の減少
	if( jump_power_ > 0 )
	{
		jump_power_ = jump_power_ - kDecay;
	}
	else
	{
		jump_power_ = 0;
		flag_.reset( Flag::kCollision );
	}

	//ブースト力の減少
	boost_power_ = boost_power_ > kSpeed ? jump_power_ - kDecay : kSpeed;

	movement_ += Vector2( std::cos( jumping_angle_ ) , -std::sin( jumping_angle_ ) ) * jump_power_;

	//座標更新
	myshape_.position += movement_;

	if( myshape_.position.y > 720.0F - 30.0F )
	{
		flag_.reset( Flag::kJump );
		myshape_.position.y = 720.0F - 30.0F;
	}

	move_vector_.end = myshape_.position;
}

void Player::draw()
{
	RECT trim;
	trim.top = 0;
	trim.left = 0;
	trim.bottom = trim.top + kPlayerSize;
	trim.right = trim.left + kPlayerSize;

	Sprite::getInstance()->draw( texture_ , myshape_.position , &trim , 1.0F , 1.0F , Vector2( 1.0F , 1.0F ) , 0.0F , Vector2( kPlayerSize / 2.0F , kPlayerSize - kPlayerSize / 4.0F ) );
}

bool Player::isLife()
{
	if( myshape_.position.y > 1200.0F )
		return false;

	return true;
}

void Player::revision(const Vector2& CrossPoint)
{
	myshape_.position = CrossPoint;
	setGravityAngle();
	dis_ = ( CrossPoint - ground_.start ).Length() / ( ground_.end - ground_.start ).Length();
	myshape_.position += Vector2( cos( gravity_angle_ + XM_PI ) , -sin( gravity_angle_ + XM_PI ) ) * myshape_.radius;
}

void Player::collision( Star * StarObj)
{
	if( owner_ != StarObj )
		owner_ = StarObj;

	jump_power_ = 0;
	flag_.reset( Flag::kJump );
}

void Player::collision( Wall * WallObj)
{
	setGround( kGround );

	//角度変更( 今は90度加減算 )
	if( myshape_.position.x < getWindowWidth<float>() / 2 )
		jumping_angle_ -= XM_PI / 2.0F;
	else if( myshape_.position.x > getWindowWidth<float>() / 2 )
		jumping_angle_ += XM_PI / 2.0F;

	flag_.reset( Flag::kCollision );
}


//----------------------------------------------
//内部利用関数
void Player::input()
{
	GamePad::State pad = Pad::getInstance()->getState();
	GamePad::ButtonStateTracker pad_tracker = Pad::getInstance()->getTracker();
	Keyboard::KeyboardStateTracker key = Key::getInstance()->getTracker();
	Vector2 stick( pad.thumbSticks.leftX , pad.thumbSticks.leftY );
	Vector2 move = movement_;
	//正規化
	stick.Normalize();
	move.Normalize();
	float cross = Calc::cross( move , stick );

	if( flag_.test( Flag::kJump ) )
	{
		//移動方向に対して入力が右の場合( 右入力時 )
		if( cross < 0 || key.lastState.Right )
		{
			movement_ += Vector2( std::cos( jumping_angle_ - XM_PI / 2.0F ) , -std::sin( jumping_angle_ - XM_PI / 2.0F ) ) * boost_power_;
		}
		//移動方向に対して入力が左の場合( 左入力時 )
		else if( cross > 0 || key.lastState.Left )
		{
			movement_ += Vector2( std::cos( jumping_angle_ + XM_PI / 2.0F ) , -std::sin( jumping_angle_ + XM_PI / 2.0F ) ) * boost_power_;
		}
		//ブースト入力
		if(( pad_tracker.a == pad_tracker.PRESSED || key.pressed.Space) &&
			!flag_.test(Flag::kBoost))
		{
			flag_.set( Flag::kBoost );
			boost_power_ = kBoostPower;
		}
	}
	else
	{
		//ジャンプ入力
		if( pad_tracker.a == pad_tracker.HELD || key.lastState.Space )
		{
			//アニメーションの処理
		}
		//ジャンプ
		if( pad_tracker.a == pad_tracker.RELEASED || key.released.Space )
		{
			flag_.set( Flag::kJump );
			flag_.set( Flag::kCollision );
			jump_power_ = kJumpPower;
			jumping_angle_ = gravity_angle_ + XM_PI;
			ground_ = kGround;
		}
	}
}

void Player::gravity()
{
	if( ground_ == kGround )
	{
		setGravityAngle();
		movement_ += Vector2( std::cos( gravity_angle_ ) , -std::sin( gravity_angle_ ) ) * kGravity;
	}
	else
		revision( ground_.start + ( ground_.end - ground_.start ) * dis_ );
}

void Player::setGravityAngle()
{
	//線分に対して±90度方向の角度を求める
	Vector2 vect_stop = ground_.end - ground_.start;
	float temp_angle[ 2 ] =
	{
		std::atan2( -vect_stop.y,vect_stop.x ) - ( XM_PI / 2.0F ),
		std::atan2( -vect_stop.y,vect_stop.x ) + ( XM_PI / 2.0F )
	};

	temp_angle[ 0 ] = static_cast<float>(static_cast<int>(temp_angle[ 0 ] * 100)) / 100.0F;
	temp_angle[ 1 ] = static_cast<float>(static_cast<int>(temp_angle[ 1 ] * 100)) / 100.0F;
	//線分に対して±90度方向に半径分移動する
	Vector2 temp_posit[ 2 ] =
	{
		( vect_stop + Vector2( std::cos( temp_angle[ 0 ] ) ,-std::sin( temp_angle[ 0 ] ) ) * 10.0F ),
		( vect_stop + Vector2( std::cos( temp_angle[ 1 ] ) ,-std::sin( temp_angle[ 1 ] ) ) * 10.0F )
	};

	//作成した各線分に対して外積を取り負の方向の角度を重力方向として採用
	vect_stop.Normalize();
	temp_posit[ 0 ].Normalize();
	temp_posit[ 1 ].Normalize();
	if( Calc::cross( vect_stop , temp_posit[ 0 ] ) > 0 )
	{
		gravity_angle_ = temp_angle[ 0 ];
		return;
	}

	gravity_angle_ = temp_angle[ 1 ];
}
