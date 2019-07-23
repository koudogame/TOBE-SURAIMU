#include "stay_particle.h"
#include "sprite.h"
#include "textureLoder.h"
#include "task_manager.h"

const int kRtate = 4;			//回転角
const int kTextureSize = 46;	//テクスチャサイズ


StayParticle::StayParticle()
{}


StayParticle::~StayParticle()
{}

//初期化
bool StayParticle::init(Vector2* Posit, const int TurnDirection)
{
	//テクスチャの取得
	texture_ = TextureLoder::getInstance()->load(L"Texture/character.png");
	turn_ = -1;
	if (TurnDirection)
		turn_ = 1;
	position_ = Posit;

	//タスクへの登録
	TaskManager::getInstance()->registerTask(this, TaskUpdate::kParticle);
	TaskManager::getInstance()->registerTask(this, TaskDraw::kParticle);
	return true;
}

//破棄
void StayParticle::destroy()
{
	TextureLoder::getInstance()->release(texture_);
	TaskManager::getInstance()->unregisterObject(this);
}

//更新
void StayParticle::update()
{
	angle_ += turn_ * kRtate;
}

//描画
void StayParticle::draw()
{
	//切り取り位置
	RECT trim;
	trim.left = kTextureSize * 2;
	trim.top = 0;
	trim.right = trim.left + kTextureSize;
	trim.bottom = trim.top + kTextureSize;
	//加算合成
	Sprite::getInstance()->reserveDraw(
		texture_,
		(*position_),
		trim,
		1.0F,
		0.9F,
		Vector2(1.0F, 1.0F),
		XMConvertToRadians(angle_) ,
		Vector2(kTextureSize / 2.0F, kTextureSize / 2.0F),
		Sprite::getInstance()->chengeMode());
}