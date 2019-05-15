
// 板場 温樹

#include "particle.h"

#include "textureLoder.h"
#include "sprite.h"

using namespace std::chrono;


/*===========================================================================*/
Particle::Particle(TaskManager* const TaskManager) :
	ObjectBase(ObjectID::kParticle, TaskManager)
{
}

Particle::~Particle()
{
}

/*===========================================================================*/
// 初期化処理
bool Particle::init(
	const wchar_t* const TextureFileName,
	const Vector2& Velocity,
	const long long LifeTimeMs)
{
	// テクスチャの読み込み
	texture_ = TextureLoder::getInstance()->load(TextureFileName);
	if (texture_ == nullptr) { return false; }


	// メンバ
	velocity_ = Velocity;
	lifetime_ms_ = LifeTimeMs;
	prev_time_ = high_resolution_clock::now();
	is_alive_ = true;


	return true;
}

/*===========================================================================*/
// 終了処理
void Particle::destroy()
{


	// テクスチャ開放
	TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
void Particle::update()
{
	auto now = high_resolution_clock::now();
	lifetime_ms_ -= duration_cast<milliseconds>(now - prev_time_).count();
	if (lifetime_ms_ <= 0LL)
	{
		is_alive_ = false;
	}
	else
	{
		position_ += velocity_;
	}
}

/*===========================================================================*/
// 描画処理
void Particle::draw()
{
	Sprite::getInstance()->draw(
		texture_,
		position_,
		nullptr
	);
}
