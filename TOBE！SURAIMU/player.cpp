#include "player.h"
#include "task_manager.h"
#include "textureLoder.h"
#include "sprite.h"
#include "pad.h"
#include "key.h"
#include "calc.h"
#include "space.h"

#include "easing.h"
#include "Sound.h"
#include "space.h"

//判定対象クラス
#include "star.h"
#include "wall.h"
#include "fail_wall.h"

const int kPlayerSize = 46;		                //テクスチャサイズ
const int kFlicTime = 18;		                //アニメーション更新時間
const int kParticleTime = 1;	                //パーティクルの生成クールフレーム
const int kBottomOn = 3;		                //下入力時の重力倍率
const int kBottomOff = 1;		                //下入力なしの重力倍率
const float kDeathLine = 720;	            //死亡ライン
const float kGuideHeight = 214.0F;	            //ガイドのテクスチャの高さ
const float kGuideWidth = 5.0F;		            //ガイドのテクスチャの幅
const int kMaxPlayer = 4;			            //最大プレイ人数

constexpr float kParticleInterval = 2.5F;       //パーティクルの生成間隔

//プレイヤーのステータス
constexpr float kJumpAmount = 900.0F;                           //プレイヤージャンプ力
constexpr float kAddVolume = 0.005F;                            //プレイヤー増加量
constexpr float kGravity = 8.0F;                                //プレイヤー重力
constexpr float kSpeed = 0.5F;                                  //プレイヤー速さ
constexpr float kMaxOffset = 5.0F;
constexpr float kReboundBasePower = 1.0F;		                //プレイヤーの反発力
constexpr float kReboundDecay = 0.05F;			                //プレイヤーの反発力の減衰量
constexpr float kMaxChangeAngle = XMConvertToRadians( 70 );		//左右の最大変角度

//コンストラクタ
Player::Player()
{
	kGround.start = Vector2( 0.0F, getWindowHeight<float>()*2.0F );
	kGround.end = Vector2( getWindowWidth<float>(), getWindowHeight<float>()*2.0F );
}

//デストラクタ
Player::~Player()
{
}


//初期化
bool Player::init( const Vector2 & Posit, const int PlayerNo )
{
	player_no_ = PlayerNo;

	//タスクへの追加
	TaskManager::getInstance()->registerTask( this, TaskUpdate::kPlayer );
	TaskManager::getInstance()->registerTask( this, TaskDraw::kObject );

	Space::getInstance()->registration( this, Posit, 5.5F );

	myshape_ = Circle( Posit, 5.5F );
	//定数の定義
	ground_ = &kGround;
	//テクスチャの読み込み
	texture_ = TextureLoder::getInstance()->load( L"Texture/character.png" );
	guide_ = TextureLoder::getInstance()->load( L"Texture/guide.png" );

	if ( texture_ == nullptr )
		return false;

	//パーティクルコンテナの取得
	g_particle_container_ = std::make_unique< GroundParticleContainer>();
	f_particle_container_ = std::make_unique<FreeFallParticleContainer>();
	s_particle_container_ = std::make_unique<StayParticleContainer>( &myshape_.position );

	//プレイヤーのパーティクル追加
	for ( int i = 0; i < 2; i++ )
		s_particle_container_.get()->addParticle( i );

	//各変数の初期化
	setGravityAngle();
	now_amount_ = 0.0F;
	particle_time_ = 0;
	prev_jump_moveamount_ = 0;
	bottom_input_ = kBottomOff;

	timer = 0;
	guide_alpha_ = 1.0F;
	particle_alpha_ = 1.0F;

	owner_ = nullptr;

	score_.init();


	died_flag_ = false;

	base_angle_ = XM_PIDIV2;
	over_ = 0.0F;
	kJumpPower = kJumpAmount;

	Space::getInstance()->registration( this, myshape_.position, myshape_.radius );

	return true;
}

//破棄
void Player::destroy()
{
	Space::getInstance()->unregistration( this );
	//全パーティクル削除
	g_particle_container_.get()->destroy();
	f_particle_container_.get()->destroy();
	s_particle_container_.get()->destroy();

	score_.destroy();

	//テクスチャの開放
	TaskManager::getInstance()->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
	TextureLoder::getInstance()->release( guide_ );
}

//更新
void Player::update()
{
	//全パーティクルの更新処理
	g_particle_container_.get()->update();
	f_particle_container_.get()->update();

	if( died_flag_ )
		return;

	//スコアの更新
	score_.update();

	move_vector_.start = myshape_.position;
	movement_ = Vector2::Zero;

	if ( !flag_.test( Flag::kJump ) )
	{
		//入力時処理
		inputjump();
	}

	//ジャンプ量を増やす
	if ( flag_.test( Flag::kJump ) )
		now_amount_ += kAddVolume;

	if ( now_amount_ >= 1.0F )
		now_amount_ = 1.0F;

	float move_power = Easing::getInstance()->expo( kJumpPower, now_amount_, Easing::Mode::Out ) - prev_jump_moveamount_;

	if (move_power - kGravity < 0 && flag_.test(Flag::kJump))
	{
		flag_.reset( Flag::kStarCollision );
		score_.resetCombo();
	}

	movement_ += ( Vector2( std::cos( base_angle_ ), -std::sin( base_angle_ ) ) * move_power );


	if ( ground_ != &kGround )
	{
		revision( ground_->start + ( ground_->end - ground_->start ) * dis_, NameSpaceParticle::ParticleID::kNonParticle );
		base_angle_ = revision_angle_;
	}
	else
		movement_ += Vector2( 0.0F, kGravity * bottom_input_ );

	prev_jump_moveamount_ = Easing::getInstance()->expo( kJumpPower, now_amount_, Easing::Mode::Out );

	if ( flag_.test( Flag::kJump ) )
		inputmove();

	if ( !flag_.test( Flag::kJump ) && owner_ != nullptr )
	{
		revision( ground_->start + ( ground_->end - ground_->start ) * dis_, NameSpaceParticle::ParticleID::kNonParticle );
		base_angle_ = revision_angle_;
	}

	myshape_.position += movement_ + offset_;

	move_vector_.end = myshape_.position;

	if ( !flag_.test( Flag::kJump ) )
		score_.addRotate( XMConvertToDegrees( getRotate() ) );

	Space::getInstance()->registration( this, myshape_.position, myshape_.radius );

	//各パーティクルの追加
	addFreeFallParticle();
}

//描画
void Player::draw()
{
	if (died_flag_)
		return;

	float draw_angle = 0.0F;

	if ( flag_.test( Flag::kJump ) )
	{
		Vector2 movement = move_vector_.end - move_vector_.start;
		draw_angle = XM_PIDIV2 - std::atan2( -movement.y, movement.x );
	}
	else
		draw_angle = -revision_angle_ - XM_PIDIV2;

	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	//ガイドの描画
	if ( !flag_.test( Flag::kJump ) && guide_alpha_ > 0.0F )
		Sprite::getInstance()->draw( guide_, myshape_.position, nullptr, guide_alpha_, 0.2F, Vector2( 1.0F, 1.0F ), XMConvertToDegrees( draw_angle ), Vector2( kGuideWidth / 2.0F, kGuideHeight ) );
	else if ( guide_alpha_ <= 0.0F )
		guide_alpha_ = 0.0F;
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();


	slectDirection();

	RECT trim;
	trim.top = static_cast< int >( rect_left_up_.y );
	trim.left = static_cast< int >( rect_left_up_.x );
	trim.bottom = trim.top + kPlayerSize;
	trim.right = trim.left + kPlayerSize;

	Sprite::getInstance()->draw(texture_, myshape_.position, &trim, 1.0F, 1.0F, Vector2(1.0F, 1.0F), XMConvertToDegrees(draw_angle), Vector2(kPlayerSize / 2.0F, kPlayerSize / 2.0F));

	score_.draw();

}

//生存フラグの返却
bool Player::isAlive()
{
	if ( died_flag_ )
	{
		owner_ = nullptr;
		ground_ = &kGround;
		return diedEffect();
	}

	return true;
}

//移動量の追加
void Player::setMove( const float Over )
{
	over_ = Over;
	myshape_.position.y += Over;
}

//座標の補正
void Player::revision( const Vector2& CrossPoint, NameSpaceParticle::ParticleID ID )
{
	myshape_.position = CrossPoint;
	if ( ID != NameSpaceParticle::ParticleID::kNonParticle )
	{
		dis_ = Calc::magnitude( CrossPoint, ground_->start ) / Calc::magnitude( ground_->end, ground_->start );
	}
	setGravityAngle();

	if ( !flag_.test( Flag::kParticle ) )
	{
		addGroundParticle( ID );
		flag_.set( Flag::kParticle );
	}
	myshape_.position += Vector2( cos( revision_angle_ + XM_PI ), -sin( revision_angle_ + XM_PI ) ) * myshape_.radius;
	move_vector_.end = myshape_.position;
}

//星との当たり判定後の処理
void Player::collision( Star * StarObj )
{
	if ( owner_ != StarObj || flag_.test( Flag::kTechnique ) )
	{
		if ( score_.isStart() )
		{
			SOUND->stop( SoundId::kJump );
			SOUND->stop( SoundId::kCllision );
			SOUND->setPitch( SoundId::kCllision, collision_combo_pitch_ );
			SOUND->play( SoundId::kCllision, false );
			collision_combo_pitch_ += 0.2F;
			if ( collision_combo_pitch_ >= 1.0F )collision_combo_pitch_ = 1.0F;
		}
		rect_left_up_ = Vector2::Zero;
		direction_id_ = Direction::kFlont;
		timer = 0;
		score_.addCombo();
		if ( score_.getLevel() >= 2 )
			guide_alpha_ -= 0.1F;

		if ( owner_ == StarObj )
			score_.addTechnique();
		else
			score_.resettechnique();
	}
	base_angle_ = revision_angle_;
	died_flag_ ? owner_ = nullptr : owner_ = StarObj;
	now_amount_ = 0.0F;
	prev_jump_moveamount_ = 0.0F;
	if (flag_.test(Flag::kJump) && owner_ != nullptr)
	{
 		kJumpPower = kJumpAmount * StarObj->getRotateDia();
		score_.addLength((myshape_.position - owner_->getPosition()).Length() / dynamic_cast<Star*>(owner_)->getSize());
	}
	flag_.reset( Flag::kJump );
	flag_.reset( Flag::kTechnique );
	flag_.reset(Flag::kWallParticle);

}

//壁との当たり判定後の処理
void Player::collision( Wall * WallObj )
{
	if ( score_.isStart() )
	{
		SOUND->stop( SoundId::kJump );
		SOUND->stop( SoundId::kCllision );
		SOUND->setPitch( SoundId::kCllision, 0.0F );
		SOUND->play( SoundId::kCllision, false );
	}
	setGround( &kGround );

	if ( !flag_.test( Flag::kWallCollision ) )
	{
		base_angle_ = XM_PI - base_angle_;
		offset_.x *= -1.0F;
	}

	flag_.set( Flag::kWallCollision );

	//角度変更
	flag_.reset( Flag::kStarCollision );
	flag_.set( Flag::kTechnique );
	if ( !flag_.test( Flag::kWallParticle ) )
		flag_.reset( Flag::kParticle );
	flag_.set( Flag::kWallParticle );
}

//プレイヤーとの当たり判定
void Player::collision( Player * PlayerObj )
{
	if ( score_.isStart() )
	{
		SOUND->stop( SoundId::kCllision );
		SOUND->setPitch( SoundId::kCllision, 0.0F );
		SOUND->play( SoundId::kCllision, false );
	}

	base_angle_ = XM_PI - base_angle_;
	offset_.x *= -1.5F;	//プレイヤーのコンボで反射率変更

	flag_.set( Flag::kPlayerCollision );

	if ( std::abs( offset_.x ) > kMaxOffset )
		offset_.x = std::copysign( kMaxOffset, offset_.x );
}

void Player::collision(FailWall* FW)
{
	if (!died_flag_)
		diedEffect();
}

//回転角を返却
float Player::getRotate()
{
	if ( owner_ == nullptr || ground_ == &kGround )
		return false;

	return abs( Calc::angle( move_vector_.start - dynamic_cast< Star* >( owner_ )->getPosition(), move_vector_.end - dynamic_cast< Star* >( owner_ )->getPosition() ) );
}


//----------------------------------------------
//内部利用関数
void Player::inputjump()
{
	offset_ = Vector2::Zero;

	GamePad::ButtonStateTracker pad_tracker = Pad::getInstance()->getTracker( player_no_ );
	Keyboard::KeyboardStateTracker key = Key::getInstance()->getTracker();
	direction_id_ = Direction::kFlont;
	//ジャンプ入力
	if ( pad_tracker.a == pad_tracker.HELD || key.lastState.Space )
	{
		//アニメーションの処理
		direction_id_ = Direction::kSquat;
	}
	//ジャンプ
	if( pad_tracker.a == pad_tracker.RELEASED || key.released.Space )
	{
		SOUND->stop( SoundId::kJump );
		SOUND->play( SoundId::kJump, false );
		flag_.set( Flag::kJump );
		flag_.set( Flag::kStarCollision );
		flag_.reset( Flag::kParticle );
		flag_.reset( Flag::kOnce );
		direction_id_ = Direction::kFlay;
		particle_time_ = 0;
		now_amount_ = 0.0F;
		base_angle_ = revision_angle_ + XM_PI;
		ground_ = &kGround;
		prev_jump_moveamount_ = 0.0F;
		score_.resetRotate();
	}
}

void Player::inputmove()
{
	GamePad::ButtonStateTracker pad_tracker = Pad::getInstance()->getTracker( player_no_ );
	Keyboard::KeyboardStateTracker key = Key::getInstance()->getTracker();

	Vector2 move = movement_ + offset_;

	float angle = std::atan2( -move.y, move.x );

	Vector2 temp = Vector2::Zero;

	//右入力
	if ( pad_tracker.dpadRight == pad_tracker.HELD || key.lastState.Right )
	{
		if ( std::cos( angle - XM_PIDIV2 ) > 0 )
			temp = Vector2( std::cos( angle - XM_PIDIV2 ), -std::sin( angle - XM_PIDIV2 ) )*kSpeed;
		else
			temp = -Vector2( std::cos( angle - XM_PIDIV2 ), -std::sin( angle - XM_PIDIV2 ) )*kSpeed;
	}
	//左入力
	else if ( pad_tracker.dpadLeft == pad_tracker.HELD || key.lastState.Left )
	{
		if ( std::cos( angle + XM_PIDIV2 ) < 0 )
			temp = Vector2( std::cos( angle + XM_PIDIV2 ), -std::sin( angle - XM_PIDIV2 ) )*kSpeed;
		else
			temp = -Vector2( std::cos( angle + XM_PIDIV2 ), -std::sin( angle + XM_PIDIV2 ) )*kSpeed;
	}

	if ( Vector2( offset_ + temp ).Length() < kMaxOffset )
		offset_ += temp;

	//下入力
	if ( pad_tracker.dpadDown == pad_tracker.HELD || key.lastState.Down )
	{
		bottom_input_ = kBottomOn;
		score_.addDown();
		score_.resetCombo();
		flag_.reset( Flag::kStarCollision );
	}
	else
		bottom_input_ = kBottomOff;
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
	if ( Calc::cross( vect_ground, temp_posit[ 0 ] ) > 0 )
	{
		revision_angle_ = temp_angle[ 0 ];
		return;
	}

	revision_angle_ = temp_angle[ 1 ];
}

//プレイヤーの状態から切り取り範囲を選択
void Player::slectDirection()
{
	switch ( direction_id_ )
	{
		case Direction::kFlont:
			if ( timer++ > kFlicTime )
			{
				timer = 0;
				rect_left_up_ = rect_left_up_ == Vector2::Zero ? Vector2( kPlayerSize, 0.F ) : Vector2::Zero;
			}

			break;

		case Direction::kSquat:
			rect_left_up_ = Vector2( 0.F, kPlayerSize );
			break;

		case Direction::kFlay:
			rect_left_up_ = Vector2( kPlayerSize, kPlayerSize );
			break;
	}
}

//オブジェクトとの衝突時のパーティクルを生成
void Player::addGroundParticle( NameSpaceParticle::ParticleID ID )
{
	if ( score_.isStart() )
	{
		//衝突時のパーティクルの生成
		g_particle_container_.get()->addParticle( myshape_.position, revision_angle_ + XM_PI + XMConvertToRadians( 45.0F ), ID );
		g_particle_container_.get()->addParticle( myshape_.position, revision_angle_ + XM_PI + XMConvertToRadians( 15.0F ), ID );
		g_particle_container_.get()->addParticle( myshape_.position, revision_angle_ + XM_PI - XMConvertToRadians( 45.0F ), ID );
		g_particle_container_.get()->addParticle( myshape_.position, revision_angle_ + XM_PI - XMConvertToRadians( 15.0F ), ID );
	}
}

//ジャンプ中に発生するパーティクルの生成
void Player::addFreeFallParticle()
{
	if ( flag_.test( Flag::kJump ) )
	{
		Vector2 nomal = move_vector_.end - move_vector_.start;
		float angle = XM_PIDIV2 - std::atan2( -nomal.y, nomal.x );
		int particle_num = static_cast< int >( ( nomal.Length() + over_ ) / kParticleInterval );

		nomal.Normalize();

		for ( int i = 0; i < particle_num + 1; i++ )
		{
			Vector2 create_position = myshape_.position - nomal * static_cast<float>(i) * kParticleInterval;
			//ジャンプ時のパーティクル生成
			f_particle_container_.get()->addParticle( create_position, NameSpaceParticle::ParticleID::kPlayer, 20.0F, false, XMConvertToDegrees(angle));
		}
	}
	else
	{
		if ( owner_ != nullptr )
		{
			dynamic_cast< Star* >( owner_ )->addFreeFallParticle();
		}
	}

	over_ = 0.0F;
}

bool Player::diedEffect()
{
	if ( !died_flag_ )
	{
		SOUND->stop( SoundId::kJump );
		SOUND->stop( SoundId::kCllision );
		SOUND->stop( SoundId::kDied );
		SOUND->play( SoundId::kDied, false );

		//元あったパーティクルは全消去
		g_particle_container_->destroy();
		s_particle_container_.get()->destroy();

		//死亡時のパーティクル( 衝突時のものを流用( Scale 2.0F ) )を生成
		g_particle_container_->addParticle( Vector2( myshape_.position.x, getWindowHeight<float>() ), XMConvertToRadians( 45 ), NameSpaceParticle::ParticleID::kCyan, 2.0F );
		g_particle_container_->addParticle( Vector2( myshape_.position.x, getWindowHeight<float>() ), XMConvertToRadians( 75 ), NameSpaceParticle::ParticleID::kMagenta, 2.0F );
		g_particle_container_->addParticle( Vector2( myshape_.position.x, getWindowHeight<float>() ), XMConvertToRadians( 105 ), NameSpaceParticle::ParticleID::kWall, 2.0F );
		g_particle_container_->addParticle( Vector2( myshape_.position.x, getWindowHeight<float>() ), XMConvertToRadians( 135 ), NameSpaceParticle::ParticleID::kYellow, 2.0F );
		died_flag_ = true;

		score_.stop();
		flag_.set( Flag::kJump );
	}

	if ( g_particle_container_->active().size() == 0 )
		return false;

	return true;
}

