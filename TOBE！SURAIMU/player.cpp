#include "player.h"
#include "task_manager.h"
#include "textureLoder.h"
#include "sprite.h"
#include "pad.h"
#include "key.h"
#include "calc.h"

#include "easing.h"

//判定対象クラス
#include "star.h"
#include "wall.h"

const int kPlayerSize = 21;
const int kFlicTime = 18;
const int kParticleTime = 10;

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
bool Player::init( const Vector2 & Posit , const float Jump , const float AddVol , const float Decay , const float Gravity , const float Speed , const float UpBoost,const float RLBoost )
{
	task_manager_->registerTask( this , TaskUpdate::kPlayerUpdate );
	task_manager_->registerTask( this , TaskDraw::kPlayerDraw );
	myshape_ = Circle( Posit , 5.5F );
	//定数の定義
	kJumpAmount = Jump;
	kAddVolume = AddVol;
	kDecay = Decay;
	kGravity = Gravity;
	kSpeed = Speed;
	kUPBoostPower = UpBoost;
	kRLBoostPower = RLBoost;
	ground_ = &kGround;
	texture_ = TextureLoder::getInstance()->load( L"Texture/player.png" );
	Num = TextureLoder::getInstance()->load( L"Texture/数字.png" );

	if( texture_ == nullptr )
		return false;

	g_particle_container_ = std::make_unique< GroundParticleContainer>( task_manager_ );
	s_particle_container_ = std::make_unique<FreeFallParticleContainer>( task_manager_ );

	//各変数の初期化
	setGravityAngle();
	jumping_angle_ = gravity_angle_ + XM_PI;
	now_amount_ = 0.0F;
	jump_power_ = kJumpAmount;
	particle_time_ = 0;

	timer = 0;

	owner_ = nullptr;

	return true;
}

//破棄
void Player::destroy()
{
	//全パーティクル削除
	g_particle_container_.get()->destroy();
	s_particle_container_.get()->destroy();

	task_manager_->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

//更新
void Player::update()
{
	//全パーティクルの更新処理
	g_particle_container_.get()->update();
	s_particle_container_.get()->update();

	//各パーティクルの追加
	addFreeFallParticle();

 	move_vector_.start = myshape_.position;

	//入力時処理
	input();

	//ジャンプ量を増やす
	if( flag_.test( Flag::kJump ) )
		now_amount_ += kAddVolume;

	if( now_amount_ >= 1.0F )
		flag_.reset( Flag::kCollision );

	//ブースト力の減少
	boost_power_ = boost_power_ > kSpeed ? boost_power_ - kDecay : kSpeed;

	myshape_.position += Vector2( std::cos( jumping_angle_ ) , -std::sin( jumping_angle_ ) ) * Easing::getInstance()->expo( jump_power_ , now_amount_ , Easing::Mode::Out );

	//重力をかける
	gravity();

	move_vector_.end = myshape_.position;
}

//描画
void Player::draw()
{
	slectDirection();
	RECT trim;
	trim.top = static_cast<int>(rect_left_up_.y);
	trim.left = static_cast< int >( rect_left_up_.x );
	trim.bottom = trim.top + kPlayerSize;
	trim.right = trim.left + kPlayerSize;

	float draw_angle;

	if( flag_.test( Flag::kJump ) )
	{
		Vector2 movement = move_vector_.end - move_vector_.start;
		draw_angle = XM_PI / 2.0F - std::atan2( -movement.y , movement.x );
	}
	else
		draw_angle = -gravity_angle_ - XM_PI / 2.0F;

	Sprite::getInstance()->draw( texture_ , myshape_.position, &trim , 1.0F , 1.0F , Vector2( 1.0F , 1.0F ) , XMConvertToDegrees( draw_angle ) , Vector2( kPlayerSize / 2.0F , kPlayerSize / 2.0F ) );
	num.draw( Num , Vector2( 1280 , 720.0F - 128.0F ) , 64L , 128L );
}

//生存フラグの返却
bool Player::isAlive()
{
	if( myshape_.position.y > 1200.0F )
		return false;

	return true;
}

void Player::setMove( const float Over )
{
	myshape_.position.y += Over;
	g_particle_container_.get()->setMove( Over );
	s_particle_container_.get()->setMove( Over );
}

//座標の補正
void Player::revision(const Vector2& CrossPoint)
{
	myshape_.position = CrossPoint;
	dis_ = Calc::magnitude( CrossPoint , ground_->start ) / Calc::magnitude( ground_->end , ground_->start );
	setGravityAngle();
	if( !flag_.test( Flag::kParticle ) )
	{
		addGroundParticle();
		flag_.set( Flag::kParticle );
	}
	myshape_.position += Vector2( cos( gravity_angle_ + XM_PI ) , -sin( gravity_angle_ + XM_PI ) ) * myshape_.radius;
	move_vector_.end = myshape_.position;
}

//星との当たり判定後の処理
void Player::collision( Star * StarObj)
{
	if( owner_ != StarObj )
	{
		rect_left_up_ = Vector2::Zero;
		direction_id_ = Direction::kFlont;
		timer = 0;
	}
	owner_ = StarObj;
	now_amount_ = 0.0F;
	flag_.reset( Flag::kJump );
}

//壁との当たり判定後の処理
void Player::collision( Wall * WallObj)
{
	setGround( &kGround );

	//角度変更
	jumping_angle_ = XM_PI - jumping_angle_;
	flag_.reset( Flag::kCollision );
	flag_.reset( Flag::kParticle );
	owner_ = nullptr;
}

//回転角を返却
float Player::getRotate()
{
	if( owner_ == nullptr || ground_ == &kGround )
		return false;

	return abs( Calc::angle( move_vector_.start - dynamic_cast< Star* >( owner_ )->getPosition() , move_vector_.end - dynamic_cast< Star* >( owner_ )->getPosition() ) );
}

//定数変数を再設定
void Player::resetStatus( const float Magnification )
{
	kJumpAmount   *= Magnification;
	kAddVolume    *= Magnification;
	kDecay        *= Magnification;
	kGravity      *= Magnification;
	kSpeed        *= Magnification;
	kUPBoostPower *= Magnification;
	kRLBoostPower *= Magnification;
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
			jump_power_ += kUPBoostPower;
			boost_power_ = kRLBoostPower;
		}
	}
	//接地中
	else
	{
		direction_id_ = Direction::kFlont;
		//ジャンプ入力
		if( pad_tracker.a == pad_tracker.HELD || key.lastState.Space )
		{
			//アニメーションの処理
			direction_id_ = Direction::kSquat;
		}
		//ジャンプ
		if( pad_tracker.a == pad_tracker.RELEASED || key.released.Space )
		{
 			flag_.set( Flag::kJump );
			flag_.set( Flag::kCollision );
			flag_.reset( Flag::kParticle );
			direction_id_ = Direction::kFlay;
			particle_time_ = 0;
			now_amount_ = 0.0F;
			jump_power_ = kJumpAmount;
			jumping_angle_ = gravity_angle_ + XM_PI;
			ground_ = &kGround;
		}
		flag_.reset( Flag::kBoost );
	}
}

//重力をかける
void Player::gravity()
{
	if( ground_ == &kGround )
	{
		setGravityAngle();
		myshape_.position += Vector2( std::cos( gravity_angle_ ) , -std::sin( gravity_angle_ ) ) * kGravity;
	}
	else
	{
		revision( ground_->start + ( ground_->end - ground_->start ) * dis_ );
	}

}

//地面の方向を求める
void Player::setGravityAngle()
{
	Vector2 vect_ground;
	//線分に対して±90度方向の角度を求める
	vect_ground = ground_->end - ground_->start;

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

//プレイヤーの状態から切り取り範囲を選択
void Player::slectDirection()
{
	switch( direction_id_ )
	{
		case Direction::kFlont:
			if( timer++ > kFlicTime )
			{
				timer = 0;
				rect_left_up_ = rect_left_up_ == Vector2::Zero ? Vector2( kPlayerSize , 0.F ) : Vector2::Zero;
			}

			break;

		case Direction::kSquat:
			rect_left_up_ = Vector2( 0.F , kPlayerSize );
			break;

		case Direction::kFlay:
			rect_left_up_ = Vector2( kPlayerSize , kPlayerSize );
			break;
	}
}

//オブジェクトとの衝突時のパーティクルを生成
void Player::addGroundParticle()
{
	g_particle_container_.get()->addParticle( L"Texture/パーティクル☆.png" , myshape_.position , gravity_angle_ + XM_PI + XMConvertToRadians( 45.0F ) );
	g_particle_container_.get()->addParticle( L"Texture/パーティクル☆.png" , myshape_.position , gravity_angle_ + XM_PI + XMConvertToRadians( 15.0F ) );
	g_particle_container_.get()->addParticle( L"Texture/パーティクル☆.png" , myshape_.position , gravity_angle_ + XM_PI - XMConvertToRadians( 45.0F ) );
	g_particle_container_.get()->addParticle( L"Texture/パーティクル☆.png" , myshape_.position , gravity_angle_ + XM_PI - XMConvertToRadians( 15.0F ) );
}

//ジャンプ中に発生するパーティクルの生成
void Player::addFreeFallParticle()
{
	if( flag_.test( Flag::kJump ) )
	{
		if( ++particle_time_ >= kParticleTime )
		{
			s_particle_container_.get()->addParticle( L"Texture/パーティクル☆.png" , myshape_.position , kGravity );
			particle_time_ = 0;
		}
	}
	else
	{
		if( owner_ != nullptr )
		{
			dynamic_cast< Star* >( owner_ )->addFreeFallParticle();
		}
	}
}
