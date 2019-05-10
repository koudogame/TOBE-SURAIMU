#include "player.h"
#include "task_manager.h"
#include "textureLoder.h"
#include "sprite.h"
#include "pad.h"
#include "key.h"
#include "calc.h"

const int kPlayerSize = 30;


Player::Player( TaskManager* const Manager ) :
	ObjectBase( ObjectID::kPlayer , Manager )
{}


Player::~Player()
{
	TextureLoder::getInstance()->release( texture_ );
}

bool Player::init( const Vector2 & Posit , const float Jump , const float Decay , const float Gravity , const float Speed , const float Boost )
{
	position_ = Posit;
	kJumpPower = Jump;
	kDecay = Decay;
	kGravity = Gravity;
	kSpeed = Speed;
	kBoostPower = Boost;
	ground_ = &kGround;

	texture_ = TextureLoder::getInstance()->load( L"Texture/motion dummy.png" );
	if( texture_ == nullptr )
		return false;

	return true;
}

void Player::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
}

void Player::update()
{
	move_vector.start = position_;
	movement_ = Vector2::Zero;

	//重力をかける
	gravity();

	//入力時処理
	input();

	//ジャンプ力の減少
	jump_power_ = jump_power_ > 0 ? jump_power_ - kDecay : 0;

	//ブースト力の減少
	boost_power_ = boost_power_ > kSpeed ? jump_power_ - kDecay : kSpeed;

	//座標更新
	position_ += movement_;

	if( position_.y > 720.0F - 30.0F )
		position_.y = 720.0F - 30.0F;

	move_vector.end = position_;
}

void Player::draw()
{
	RECT trim;
	trim.top = 0;
	trim.left = 0;
	trim.bottom = trim.top + kPlayerSize;
	trim.right = trim.left + kPlayerSize;

	Sprite::getInstance()->draw( texture_ , position_ , &trim , 1.0F , 1.0F , Vector2( 1.0F , 1.0F ) , 0.0F , Vector2( kPlayerSize / 2.0F , kPlayerSize - kPlayerSize / 4.0F ) );
}

bool Player::isLife()
{
	if( position_.y > 1200.0F )
		return false;

	return true;
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
		//移動方向に対して入力が左側の場合( 左入力時 )
		if( cross > 0 || key.lastState.Left )
		{
			movement_ = Vector2( std::cos( jumping_angle_ - XM_PI / 2.0F ) , -std::sin( jumping_angle_ - XM_PI / 2.0F ) ) * boost_power_;
		}
		//移動方向に対して入力が右の場合( 右入力時 )
		else if( cross < 0 || key.lastState.Right )
		{
			movement_ = Vector2( std::cos( jumping_angle_ + XM_PI / 2.0F ) , -std::sin( jumping_angle_ + XM_PI / 2.0F ) ) * boost_power_;
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
			jump_power_ = kJumpPower;
			jumping_angle_ = gravity_angle_ + XM_PI;
			ground_ = &kGround;
		}
	}
}

void Player::gravity()
{
	setGravityAngle();
	movement_ += Vector2( std::cos( gravity_angle_ ) , -std::sin( gravity_angle_ ) ) * kGravity;
}

void Player::setGravityAngle()
{
	//線分に対して±90度方向の角度を求める
	Vector2 vect_stop = ( *ground_ ).end - ( *ground_ ).start;
	float temp_angle[ 2 ] =
	{
		std::atan2( -vect_stop.y,vect_stop.x ) - ( XM_PI / 2.0F ),
		std::atan2( -vect_stop.y,vect_stop.x ) + ( XM_PI / 2.0F )
	};
	//線分に対して±90度方向に半径分移動する
	Vector2 temp_posit[ 2 ] =
	{
		( vect_stop + Vector2( std:: cos( temp_angle[ 0 ] ) ,-std::sin( temp_angle[ 0 ] ) ) * 10.0F ),
		( vect_stop + Vector2( std::cos( temp_angle[ 1 ] ) ,-std::sin( temp_angle[ 1 ] ) ) * 10.0F )
	};

	//作成した各線分に対して外積を取り負の方向の角度を重力方向として採用
	vect_stop.Normalize();
	temp_posit[ 0 ].Normalize();
	if( Calc::cross( vect_stop , temp_posit[ 0 ] ) < 0 )
	{
		gravity_angle_ = temp_angle[ 0 ];
		return;
	}

	gravity_angle_ = temp_angle[ 1 ];
}
