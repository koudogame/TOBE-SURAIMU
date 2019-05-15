
// 板場 温樹

#include "particle.h"

#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "timer.h"

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
	const Vector2& Position, const Vector2& Velocity,
	const Timer<Seconds>& Clock,
	const long long LifeTimeSec)
{
	// テクスチャの読み込み
	texture_ = TextureLoder::getInstance()->load(TextureFileName);
	if (texture_ == nullptr) { return false; }

	// タスクの登録
	task_manager_->registerTask(this, TaskUpdate::kParticleUpdate);
	task_manager_->registerTask(this, TaskDraw::kParticleDraw);


	// メンバ
	position_ = Position;
	velocity_ = Velocity;
	clock_ = &Clock;
	lifetime_sec_ = LifeTimeSec;
	is_alive_ = true;


	return true;
}

/*===========================================================================*/
// 終了処理
void Particle::destroy()
{
	// タスク登録解除
	task_manager_->unregisterObject(this);

	// テクスチャ開放
	TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
void Particle::update()
{
	if (clock_->getCount() >= lifetime_sec_)
	{
		is_alive_ = false;
	}
	else 
	{
		position_ += velocity_;
		if (position_.x < 0.0F || position_.x > getWindowWidth<float>() ||
			position_.y < 0.0F || position_.y > getWindowHeight<float>())
		{
			is_alive_ = false;
		}
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
