#include "groundParticle.h"
#include "textureLoder.h"
#include "easing.h"
#include "sprite.h"
#include "task_manager.h"

const int kTextureSize = 64;		//テクスチャサイズ

GroundParticle::GroundParticle()
{}

GroundParticle::~GroundParticle()
{}

//初期化
bool GroundParticle::init( const Vector2 & Posit , const RECT & Triming , const float Angle ,const float Scale)
{
	//タスクへの追加
	TaskManager::getInstance()->registerTask( this , TaskUpdate::kParticleUpdate );
	TaskManager::getInstance()->registerTask( this , TaskDraw::kParticle );
	//テクスチャの読み込み
	texture_ = TextureLoder::getInstance()->load( L"Texture/Particle.png" );
	if( texture_ == nullptr )
		return false;

	position_ = Posit;
	angle_ = Angle;
	now_time_ = 0;
	triming_ = Triming;
	scale_ = Scale;

	return true;
}

//破棄
void GroundParticle::destroy()
{
	TaskManager::getInstance()->unregisterObject( this );
	TextureLoder::getInstance()->release( texture_ );
}

//更新
void GroundParticle::update()
{
	if(now_time_ > 0.0F)
		velocity_ = Vector2( std::cos( angle_ ) , -std::sin( angle_ ) ) * Easing::getInstance()->expo( 100.0F , now_time_ , Easing::Mode::Out );
	now_time_ += 0.02F;
}

//描画
void GroundParticle::draw()
{
	Sprite::getInstance()->draw( texture_ , position_ + velocity_ , &triming_ , 1.0F - now_time_ , 0.2F , Vector2( scale_ , scale_ ) , 0.0F , Vector2( kTextureSize / 2.0F , kTextureSize / 2.0F ) );
}

//生存確認
bool GroundParticle::isAlive()
{
	//指定移動量移動完了
	if( now_time_ >= 1.0F )
		return false;

	return true;
}
