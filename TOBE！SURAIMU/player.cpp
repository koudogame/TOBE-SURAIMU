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

//コンストラクタ
Player::Player( TaskManager* const Manager ) :
	ObjectBase( ObjectID::kPlayer , Manager )
{
	kGround.start = Vector2( 0.0F , getWindowHeight<float>() );
	kGround.end = Vector2( getWindowWidth<float>() , getWindowHeight<float>() );
}

//デストラクタ
Player::~Player()
{
	TextureLoder::getInstance()->release( texture_ );
}


//初期化
bool Player::init( const Vector2 & Posit , const float Jump , const float Decay , const float Gravity , const float Speed , const float Boost )
{
	task_manager_->registerTask( this , TaskUpdate::kPlayerUpdate );
	task_manager_->registerTask( this , TaskDraw::kPlayerDraw );
	myshape_ = Circle( Posit , 7.5F );
	//定数の定義
	kJumpPower = Jump;
	kDecay = Decay;
	kGravity = Gravity;
	kSpeed = Speed;
	kBoostPower = Boost;
	ground_ = kGround;
	texture_ = TextureLoder::getInstance()->load( L"Texture/motion dummy.png" );
	Num = TextureLoder::getInstance()->load( L"Texture/数字.png" );

	if( texture_ == nullptr )
		return false;

	//各変数の初期化
	setGravityAngle();
	jumping_angle_ = gravity_angle_ + XM_PI;
	jump_power_ = 0;

	owner_ = nullptr;

	return true;
}

//破棄
void Player::destroy()
{
	task_manager_->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

//更新
void Player::update()
{
wo 	move_vector_.start = myshape_.position;

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

	myshape_.position += Vector2( std::cos( jumping_angle_ ) , -std::sin( jumping_angle_ ) ) * jump_power_;

	//重力をかける
	gravity();

	if( myshape_.position.y > 720.0F - 30.0F )
	{
		flag_.reset( Flag::kJump );
		myshape_.position.y = 720.0F - 30.0F;
	}

	move_vector_.end = myshape_.position;
}

//描画
void Player::draw()
{
	RECT trim;
	trim.top = 0;
	trim.left = 0;
	trim.bottom = trim.top + kPlayerSize;
	trim.right = trim.left + kPlayerSize;

	Sprite::getInstance()->draw( texture_ , myshape_.position , &trim , 1.0F , 1.0F , Vector2( 1.0F , 1.0F ) , 0.0F , Vector2( kPlayerSize / 2.0F , kPlayerSize - kPlayerSize / 4.0F ) );
	num.draw( Num , Vector2( 1280 , 720.0F - 128.0F ) , 64L , 128L );
}

//生存フラグの返却
bool Player::isLife()
{
	if( myshape_.position.y > 1200.0F )
		return false;

	return true;
}

//座標の補正
void Player::revision(const Vector2& CrossPoint)
{
	myshape_.position = CrossPoint;
	dis_ = Calc::magnitude( CrossPoint , ground_.start ) / Calc::magnitude( ground_.end , ground_.start );
	setGravityAngle();
	myshape_.position += Vector2( cos( gravity_angle_ + XM_PI ) , -sin( gravity_angle_ + XM_PI ) ) * myshape_.radius;
	move_vector_.end = myshape_.position;
}

//星との当たり判定後の処理
void Player::collision( Star * StarObj)
{
	owner_ = StarObj;
	jump_power_ = 0;
	flag_.reset( Flag::kJump );
}

//壁との当たり判定後の処理
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

//回転角を返却
float Player::getRotate()
{
	if( owner_ == nullptr || ground_ == kGround )
		return false;

	return abs( Calc::angle( move_vector_.start - dynamic_cast< Star* >( owner_ )->getposition() , move_vector_.end - dynamic_cast< Star* >( owner_ )->getposition() ) );
}


//----------------------------------------------
//内部利用関数
//入力処理
void Player::input()
{
	GamePad::State pad = Pad::getInstance()->getState();
	GamePad::ButtonStateTracker pad_tracker = Pad::getInstance()->getTracker();
	Keyboard::KeyboardStateTracker key = Key::getInstance()->getTracker();
	Vector2 stick( pad.thumbSticks.leftX , pad.thumbSticks.leftY );
	Vector2 move = myshape_.position - move_vector_.start;
	//正規化
	stick.Normalize();
	move.Normalize();
	float cross = Calc::cross( move , stick );

	//ジャンプ中
	if( flag_.test( Flag::kJump ) )
	{
		//移動方向に対して入力が右の場合( 右入力時 )
		if( cross < 0 || key.lastState.Right )
		{
			myshape_.position += Vector2( std::cos( jumping_angle_ - XM_PI / 2.0F ) , -std::sin( jumping_angle_ - XM_PI / 2.0F ) ) * boost_power_;
		}
		//移動方向に対して入力が左の場合( 左入力時 )
		else if( cross > 0 || key.lastState.Left )
		{
			myshape_.position += Vector2( std::cos( jumping_angle_ + XM_PI / 2.0F ) , -std::sin( jumping_angle_ + XM_PI / 2.0F ) ) * boost_power_;
		}
		//ブースト入力
		if(( pad_tracker.a == pad_tracker.PRESSED || key.pressed.Space) &&
			!flag_.test(Flag::kBoost))
		{
			flag_.set( Flag::kBoost );
			boost_power_ = kBoostPower;
		}
	}
	//接地中
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
		flag_.reset( Flag::kBoost );
	}
}

//重力をかける
void Player::gravity()
{
	if( ground_ == kGround )
	{
		setGravityAngle();
		myshape_.position += Vector2( std::cos( gravity_angle_ ) , -std::sin( gravity_angle_ ) ) * kGravity;
	}
	else
	{
		revision( ground_.start + ( ground_.end - ground_.start ) * dis_ );
	}

}

//地面の方向を求める
void Player::setGravityAngle()
{
	Vector2 vect_ground;
	//線分に対して±90度方向の角度を求める
	vect_ground = ground_.end - ground_.start;

	float temp_angle[ 2 ] =
	{
		std::atan2( -vect_ground.y,vect_ground.x ) - ( XM_PI / 2.0F ),
		std::atan2( -vect_ground.y,vect_ground.x ) + ( XM_PI / 2.0F )
	};
	temp_angle[ 0 ] = static_cast< float >( static_cast< int >( temp_angle[ 0 ] * 100 ) ) / 100.0F;
	temp_angle[ 1 ] = static_cast< float >( static_cast< int >( temp_angle[ 1 ] * 100 ) ) / 100.0F;
	//線分に対して±90度方向に半径分移動する
	Vector2 temp_posit[ 2 ] =
	{
		( vect_ground + Vector2( std::cos( temp_angle[ 0 ] ) ,-std::sin( temp_angle[ 0 ] ) ) * 10.0F ),
		( vect_ground + Vector2( std::cos( temp_angle[ 1 ] ) ,-std::sin( temp_angle[ 1 ] ) ) * 10.0F )
	};

	//作成した各線分に対して外積を取り負の方向の角度を重力方向として採用
	vect_ground.Normalize();
	temp_posit[ 0 ].Normalize();
	temp_posit[ 1 ].Normalize();
	num = std::abs( static_cast< long >( XMConvertToDegrees( temp_angle[ 0 ] ) ) );
	if( Calc::cross( vect_ground , temp_posit[ 0 ] ) > 0 )
	{
		gravity_angle_ = temp_angle[ 0 ];
		return;
	}

	gravity_angle_ = temp_angle[ 1 ];
}
