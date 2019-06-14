#include "freeFallParticle.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"

const float kMinFall = 0.0F;		//最小落下速度
const int kTextureSize = 64;		//テクスチャサイズ
const int kRotate = 5;				//回転角
const int kStarWhileAngle = 72;		//星の頂点の角度の差
const float kFall = 2.0F;
const float kFalldecay = 0.05F;

FreeFallParticle::FreeFallParticle()
{}


FreeFallParticle::~FreeFallParticle()
{}

//初期化
bool FreeFallParticle::init(const Vector2& Posit , const RECT& Triming )
{
	//テクスチャの読み込み
	texture_ = TextureLoder::getInstance()->load( L"Texture/Particle.png" );
	if( texture_ == nullptr )
		return false;

	//タスクへの登録
	TaskManager::getInstance()->registerTask( this , TaskUpdate::kParticle );
	TaskManager::getInstance()->registerTask( this , TaskDraw::kParticle );

	position_ = Posit;
	alpha_ = 1.0F;
	turn_ = rand() % 2 ? true : false;
	rotate_ = rand() % kStarWhileAngle;
	triming_ = Triming;
	fall_ = kFall;

	return true;
}

//解放
void FreeFallParticle::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
	TaskManager::getInstance()->unregisterObject( this );
}

//更新
void FreeFallParticle::update()
{
	alpha_ -= 0.01F;
	rotate_ += turn_ ? kRotate : -kRotate;
	position_.y += fall_;
	fall_ -= kFalldecay;

	if (fall_ < 0.0F)
		fall_ = 0.0F;
}

//描画
void FreeFallParticle::draw()
{
	//加算モードで描画
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin( Sprite::getInstance()->chengeMode() );
	Sprite::getInstance()->draw( texture_ , position_ , &triming_ , alpha_ , 0.2F , Vector2( 1.0F , 1.0F ) , static_cast< float >( rotate_ ) , Vector2( kTextureSize / 2.0F , kTextureSize / 2.0F ) );
	Sprite::getInstance()->end();
	Sprite::getInstance()->begin();
}

//生存確認
bool FreeFallParticle::isAlive()
{
	//アルファの消滅で死亡
	if( alpha_ <= 0.0F )
		return false;

	return true;
}
