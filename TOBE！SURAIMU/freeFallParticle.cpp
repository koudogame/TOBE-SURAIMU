#include "freeFallParticle.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"

const int kTextureSize = 128;		//テクスチャサイズ
const int kRotate = 5;				//回転角
const int kStarWhileAngle = 72;		//星の頂点の角度の差
const float kFall = 1.0F;
const float kFalldecay = 0.01F;

FreeFallParticle::FreeFallParticle()
{}


FreeFallParticle::~FreeFallParticle()
{}

//初期化
bool FreeFallParticle::init( const Vector2& Posit, const RECT& Triming, float LifeTime, bool RotateFlag, float Angle )
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
	max_life_ = life_time_ = LifeTime;
	rotate_ = 0.0F;
	turn_ = 0;
	if( RotateFlag )
		turn_ = rand() % 2 ? 1 : -1;
	rotate_ = Angle;
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
	alpha_ =  life_time_--/max_life_;
	rotate_ += static_cast<float>(turn_ * kRotate);
	position_.y += fall_;
	fall_ -= kFalldecay;

	if (fall_ < 0.0F)
		fall_ = 0.0F;
}

//描画
void FreeFallParticle::draw()
{
	//加算モードで描画
	Sprite::getInstance()->reserveDraw(texture_, position_, triming_, alpha_, 0.2F, Vector2(1.0F, 1.0F), XMConvertToRadians( rotate_), Vector2(kTextureSize / 2.0F, kTextureSize / 2.0F), Sprite::getInstance()->chengeMode());
}

//生存確認
bool FreeFallParticle::isAlive()
{
	//アルファの消滅で死亡または画面外で死亡
	if ( position_.y - kTextureSize / 2.0F > getWindowHeight<float>()||
		 life_time_ <= 0.0F)
		return false;

	return true;
}

void FreeFallParticle::setMove( const float Over )
{
	position_.y += Over;
}
