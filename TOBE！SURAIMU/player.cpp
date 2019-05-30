#include "player.h"
#include "task_manager.h"
#include "textureLoder.h"
#include "sprite.h"
#include "pad.h"
#include "key.h"
#include "calc.h"

#include "easing.h"
#include "Sound.h"

//判定対象クラス
#include "star.h"
#include "wall.h"

const int kPlayerSize = 46;
const int kFlicTime = 18;
const int kParticleTime = 1;
const int kBottomOn = 3;
const int kBottomOff = 1;
const float kDeathLine = 1000.0F;

//コンストラクタ
Player::Player()
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
bool Player::init( const Vector2 & Posit , const float Jump , const float AddVol , const float Decay , const float Gravity , const float Speed ,const float RLBoost )
{
	TaskManager::getInstance()->registerTask( this , TaskUpdate::kPlayerUpdate );
	TaskManager::getInstance()->registerTask( this , TaskDraw::kObject );
	myshape_ = Circle( Posit , 5.0F );
	//定数の定義
	kJumpAmount = Jump;
	kAddVolume = AddVol;
	kDecay = Decay;
	kGravity = Gravity;
	kSpeed = Speed;
	kRLBoostPower = RLBoost;
	ground_ = &kGround;
	texture_ = TextureLoder::getInstance()->load( L"Texture/character.png" );
	guide_ = TextureLoder::getInstance()->load( L"Texture/guide.png" );

	if( texture_ == nullptr )
		return false;

	g_particle_container_ = std::make_unique< GroundParticleContainer>();
	f_particle_container_ = std::make_unique<FreeFallParticleContainer>();
	s_particle_container_ = std::make_unique<StayParticleContainer>( &myshape_.position );

	for( int i = 0; i < 2; i++ )
		s_particle_container_.get()->addParticle( i );

	//各変数の初期化
	setGravityAngle();
	jumping_angle_ = gravity_angle_ + XM_PI;
	now_amount_ = 0.0F;
	particle_time_ = 0;
	prev_jump_moveamount_ = 0;
	magnification_ = 1.0F;
	bottom_input_ = kBottomOff;

	timer = 0;
	guide_alpha_ = 1.0F;

	owner_ = nullptr;

	score_.init();


	died_flag_ = false;

	return true;
}

//破棄
void Player::destroy()
{
	//全パーティクル削除
	g_particle_container_.get()->destroy();
	f_particle_container_.get()->destroy();
	s_particle_container_.get()->destroy();

	score_.destroy();

	TaskManager::getInstance()->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

//更新
void Player::update()
{
	//全パーティクルの更新処理
	g_particle_container_.get()->update();
	f_particle_container_.get()->update();

	//各パーティクルの追加
	addFreeFallParticle();

	//スコアの更新
	score_.update();

 	move_vector_.start = myshape_.position;

	//入力時処理
	input();

	//ジャンプ量を増やす
	if( flag_.test( Flag::kJump ) )
		now_amount_ += (kAddVolume * magnification_);

	if( Easing::getInstance()->expo( kJumpAmount , now_amount_ , Easing::Mode::Out ) - prev_jump_moveamount_ < kGravity )
	{
		flag_.reset( Flag::kStarCollision );
		if( flag_.test( Flag::kJump ) )
		{
			collision_combo_pitch_ = 0.0F;
			score_.resetCombo();
		}
	}

	//ブースト力の減少
	boost_power_ = boost_power_ > ( kSpeed*magnification_ ) ? boost_power_ - ( kDecay *magnification_ ) : ( kSpeed*magnification_ );

	myshape_.position += Vector2( std::cos( jumping_angle_ ) , -std::sin( jumping_angle_ ) ) * ( Easing::getInstance()->expo( kJumpAmount , now_amount_ , Easing::Mode::Out ) - prev_jump_moveamount_ );
	prev_jump_moveamount_ = Easing::getInstance()->expo( kJumpAmount , now_amount_ , Easing::Mode::Out );

	//重力をかける
	gravity();

	move_vector_.end = myshape_.position;

	if( !flag_.test( Flag::kJump ) )
		score_.addRotate( XMConvertToDegrees( getRotate() ) );
}

//描画
void Player::draw()
{
	float draw_angle;

	if( flag_.test( Flag::kJump ) )
	{
		Vector2 movement = move_vector_.end - move_vector_.start;
		draw_angle = XM_PI / 2.0F - std::atan2( -movement.y , movement.x );
	}
	else
		draw_angle = -gravity_angle_ - XM_PI / 2.0F;

	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	//ガイドの描画
	if( !flag_.test( Flag::kJump ) && guide_alpha_ > 0.0F )
		Sprite::getInstance()->draw( guide_ , myshape_.position , nullptr , guide_alpha_ , 0.2F , Vector2( 1.0F , 1.0F ) , XMConvertToDegrees( draw_angle ) , Vector2( 5.0F / 2.0F , 214.0F ) );
	else if( guide_alpha_ <= 0.0F )
		guide_alpha_ = 0.0F;
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();


	slectDirection();
	RECT trim;
	trim.top = static_cast< int >( rect_left_up_.y );
	trim.left = static_cast< int >( rect_left_up_.x );
	trim.bottom = trim.top + kPlayerSize;
	trim.right = trim.left + kPlayerSize;


	Sprite::getInstance()->draw( texture_ , myshape_.position , &trim , 1.0F , 1.0F , Vector2( 1.0F , 1.0F ) , XMConvertToDegrees( draw_angle ) , Vector2( kPlayerSize / 2.0F , kPlayerSize / 2.0F ) );

	score_.draw();

}

//生存フラグの返却
bool Player::isAlive()
{
	if( myshape_.position.y > kDeathLine )
		return diedEffect();

	return true;
}

void Player::setMove( const float Over )
{
	myshape_.position.y += Over;
}

//座標の補正
void Player::revision( const Vector2& CrossPoint , NameSpaceParticle::ParticleID ID )
{
	myshape_.position = CrossPoint;
	if( ID != NameSpaceParticle::ParticleID::kNonParticle )
	{
		dis_ = Calc::magnitude( CrossPoint , ground_->start ) / Calc::magnitude( ground_->end , ground_->start );
	}
	setGravityAngle();

	if( !flag_.test( Flag::kParticle ) )
	{
		addGroundParticle( ID );
		flag_.set( Flag::kParticle );
	}
	myshape_.position += Vector2( cos( gravity_angle_ + XM_PI ) , -sin( gravity_angle_ + XM_PI ) ) * myshape_.radius;
	move_vector_.end = myshape_.position;
}

//星との当たり判定後の処理
void Player::collision( Star * StarObj)
{

	if( owner_ != StarObj || flag_.test( Flag::kTechnique ) )
	{
		if( score_.isStart() )
		{
			SOUND->stop( SoundId::kJump );
			SOUND->stop( SoundId::kCllision );
			SOUND->setPitch( SoundId::kCllision , collision_combo_pitch_ );
			SOUND->play( SoundId::kCllision , false );
			collision_combo_pitch_ += 0.2F;
			if( collision_combo_pitch_ >= 1.0F )collision_combo_pitch_ = 1.0F;
		}
		rect_left_up_ = Vector2::Zero;
		direction_id_ = Direction::kFlont;
		timer = 0;
		score_.addCombo();
		if( score_.getLevel() >= 2 )
			guide_alpha_ -= 0.1F;

		if( owner_ == StarObj )
			score_.addTechnique();
		else
			score_.resettechnique();
	}
	owner_ = StarObj;
	now_amount_ = 0.0F;
	if( flag_.test( Flag::kJump ) )
		score_.addLength( ( myshape_.position - owner_->getPosition() ).Length() / dynamic_cast< Star* >( owner_ )->getSize() );
	flag_.reset( Flag::kJump );
	flag_.reset( Flag::kTechnique);
	flag_.reset( Flag::kWallParticle );
}

//壁との当たり判定後の処理
void Player::collision( Wall * WallObj)
{
	if( score_.isStart() )
	{
		SOUND->stop( SoundId::kJump );
		SOUND->stop( SoundId::kCllision );
		SOUND->setPitch( SoundId::kCllision , 0.0F );
		SOUND->play( SoundId::kCllision , false );
	}
	setGround( &kGround );

	//角度変更
	jumping_angle_ = XM_PI - jumping_angle_;
	flag_.reset( Flag::kStarCollision );
	flag_.set( Flag::kTechnique );
	if( !flag_.test( Flag::kWallParticle ) )
		flag_.reset( Flag::kParticle );
	flag_.set( Flag::kWallParticle );
}

//回転角を返却
float Player::getRotate()
{
	if( owner_ == nullptr || ground_ == &kGround )
		return false;

	return abs( Calc::angle( move_vector_.start - dynamic_cast< Star* >( owner_ )->getPosition() , move_vector_.end - dynamic_cast< Star* >( owner_ )->getPosition() ) );
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

	//ジャンプ中
	if( flag_.test( Flag::kJump ) )
	{
		//右入力
		if( stick.x > 0.3F || key.lastState.Right )
			myshape_.position.x += boost_power_;
		//左入力
		else if( stick.x < -0.3F || key.lastState.Left )
			myshape_.position.x -= boost_power_;

		//下入力
		if( stick.y < -0.3F || key.lastState.Down )
		{
			bottom_input_ = kBottomOn;
			score_.addDown();
			score_.resetCombo();
			flag_.reset( Flag::kStarCollision );
		}
		else
			bottom_input_ = kBottomOff;

		//ブースト入力
		if(( pad_tracker.a == pad_tracker.PRESSED || key.pressed.Space) &&
			!flag_.test(Flag::kBoost))
		{
			flag_.set( Flag::kBoost );
			boost_power_ = ( kRLBoostPower*magnification_ );
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
			if( !died_flag_ )
			{
				SOUND->stop( SoundId::kJump );
				SOUND->play( SoundId::kJump , false );
				flag_.set( Flag::kJump );
				flag_.set( Flag::kStarCollision );
				flag_.reset( Flag::kParticle );
				direction_id_ = Direction::kFlay;
				particle_time_ = 0;
				now_amount_ = 0.0F;
				jumping_angle_ = gravity_angle_ + XM_PI;
				ground_ = &kGround;
				prev_jump_moveamount_ = 0;
				score_.resetRotate();
			}
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
		myshape_.position += Vector2( std::cos( gravity_angle_ ) , -std::sin( gravity_angle_ ) ) * (kGravity *magnification_ * bottom_input_);
	}
	else
	{
		revision( ground_->start + ( ground_->end - ground_->start ) * dis_ , NameSpaceParticle::ParticleID::kNonParticle );
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
		std::atan2( -vect_ground.y,vect_ground.x ) - XM_PIDIV2,
		std::atan2( -vect_ground.y,vect_ground.x ) + XM_PIDIV2
	};
	temp_angle[ 0 ] = static_cast< float >( static_cast< int >( temp_angle[ 0 ] * 100 ) ) / 100.0F;
	temp_angle[ 1 ] = static_cast< float >( static_cast< int >( temp_angle[ 1 ] * 100 ) ) / 100.0F;
	//線分に対して±90度方向に半径分移動する
	Vector2 temp_posit[ 2 ] =
	{
		( Vector2( std::cos( temp_angle[ 0 ] ) ,-std::sin( temp_angle[ 0 ] ) ) * 10.0F ),
		( Vector2( std::cos( temp_angle[ 1 ] ) ,-std::sin( temp_angle[ 1 ] ) ) * 10.0F )
	};

	//作成した各線分に対して外積を取り負の方向の角度を重力方向として採用
	vect_ground.Normalize();
	temp_posit[ 0 ].Normalize();
	temp_posit[ 1 ].Normalize();
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
void Player::addGroundParticle( NameSpaceParticle::ParticleID ID)
{
	if( score_.isStart() )
	{
		g_particle_container_.get()->addParticle( myshape_.position , gravity_angle_ + XM_PI + XMConvertToRadians( 45.0F ) , ID );
		g_particle_container_.get()->addParticle( myshape_.position , gravity_angle_ + XM_PI + XMConvertToRadians( 15.0F ) , ID );
		g_particle_container_.get()->addParticle( myshape_.position , gravity_angle_ + XM_PI - XMConvertToRadians( 45.0F ) , ID );
		g_particle_container_.get()->addParticle( myshape_.position , gravity_angle_ + XM_PI - XMConvertToRadians( 15.0F ) , ID );
	}
}

//ジャンプ中に発生するパーティクルの生成
void Player::addFreeFallParticle()
{
	if( flag_.test( Flag::kJump ) )
	{
		if( ++particle_time_ >= kParticleTime )
		{
			f_particle_container_.get()->addParticle( myshape_.position , NameSpaceParticle::ParticleID::kPlayer );
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

bool Player::diedEffect()
{
	if( !died_flag_ )
	{
		SOUND->stop( SoundId::kJump );
		SOUND->stop( SoundId::kCllision );
		SOUND->stop( SoundId::kDied );
		SOUND->play( SoundId::kDied , false );

		g_particle_container_->addParticle( Vector2( myshape_.position.x , getWindowHeight<float>() ) , XMConvertToRadians( 45 ) , NameSpaceParticle::ParticleID::kCyan , 2.0F );
		g_particle_container_->addParticle( Vector2( myshape_.position.x , getWindowHeight<float>() ) , XMConvertToRadians( 75 ) , NameSpaceParticle::ParticleID::kMagenta , 2.0F );
		g_particle_container_->addParticle( Vector2( myshape_.position.x , getWindowHeight<float>() ) , XMConvertToRadians( 105 ) , NameSpaceParticle::ParticleID::kWall , 2.0F );
		g_particle_container_->addParticle( Vector2( myshape_.position.x , getWindowHeight<float>() ) , XMConvertToRadians( 135 ) , NameSpaceParticle::ParticleID::kYellow , 2.0F );
		died_flag_ = true;
		score_.stop();
		flag_.set( Flag::kJump );
	}

	if( g_particle_container_->active().size() == 0 )
		return false;


	return true;
}
