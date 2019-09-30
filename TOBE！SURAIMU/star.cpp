#include "star.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "calc.h"
#include "space.h"

//衝突対象クラス
#include "ai_demo.h"
#include "fail_wall.h"

//定数
const float kStarVertexLengthDeff = 0.388F;
const int kStarMin = 60;			//最小サイズ
const int kStarDifference = 20;		//星のサイズの差分
const float kMinSpin = 2.0F;		//最低回転量
const float kDeathLine = 1200.0F;	//星の死亡ライン
const float kMaxSpin[3] =			//最大回転量
{
	3.0F,
	6.0F,
	9.0F
};
const int kParticleTime = 2;		//パーティクルの生成時間
const float kFallSpeed = 2.0F;		//星の落下速度
const Vector3 kStarInformation[3] = {	//星の切り取り情報( x,y,size )
	Vector3(0.0F,0.0F,150.0F),
	Vector3(150.0F,0.0F,226.0F),
	Vector3(150.0F + 226.0F,0.0F,300.F)
};
const float kBandWidth = 100.0F;
const float kBandHeight = 300.0F;
const float kBandInterval = 0.0F;

const float kStarColorThreshold[2] =		//色変更の閾値
{
	0.5F,
	0.8F
};

Star::Star()
{
	for (float& itr : angle_)
		itr = 0;

	fall_ = 0;
	spin_ = 0;
	rate_ = 0;
	size_ = 0;
}

Star::~Star()
{}

//初期化
bool Star::init(const Vector2 & Position, const float Angle, const float Spin, const float Rate, const float Size, , const int Shape)
{
	//タスクへの登録
	TaskManager::getInstance()->registerTask(this, TaskUpdate::kStar);
	TaskManager::getInstance()->registerTask(this, TaskDraw::kParticle);

	Space::getInstance()->registration(this, Position, Size);

	//テクスチャの読み込み
	texture_ = TextureLoder::getInstance()->load(L"Texture/star.png");
	overlay_texture_ = TextureLoder::getInstance()->load(L"Texture/star1.png");

	if (texture_ == nullptr || overlay_texture_ == nullptr)
		return false;

	//落下パーティクルコンテナの取得
	s_particle_container_ = std::make_unique<FreeFallParticleContainer>();

	position_ = Position;
	spin_ = Spin;
	rate_ = Rate;
	size_ = Size;

	shape_num_ = Shape;

	myshape_.resize(shape_num_ * 2);
	angle_.resize(shape_num_ * 2);

	if (shape_num_ != 0)
		angle_[0] = Angle;

	if (Rate < kStarColorThreshold[0])
		id_ = NameSpaceParticle::ParticleID::kCyan;
	else if (Rate < kStarColorThreshold[1])
		id_ = NameSpaceParticle::ParticleID::kYellow;
	else
		id_ = NameSpaceParticle::ParticleID::kMagenta;

	particle_time_ = 0;
	alive_flag_ = true;

	setAngle();
	band_.init(id_, (size_ - kStarMin) / kStarDifference);

	return true;
}

//破棄
void Star::destroy()
{
	Space::getInstance()->unregistration(this);

	s_particle_container_.get()->destroy();
	TaskManager::getInstance()->unregisterObject(this);
	band_.destroy();
	TextureLoder::getInstance()->release(texture_);
	TextureLoder::getInstance()->release(overlay_texture_);
}

//更新
void Star::update()
{
	s_particle_container_.get()->update();

	position_.y += fall_;
	if (shape_num_ != 0)
		angle_[0] += spin_;
	setAngle();
	band_.update();

	Space::getInstance()->registration(this, position_, size_);
}

//描画
void Star::draw()
{
	if (position_.y < -size_ ||
		position_.y > getWindowHeight<float>() + size_ + band_.getHeight() ||
		static_cast<int>(size_) == 0)
		return;

	//星の描画
	RECT trim;
	trim.left = static_cast<long>(kStarInformation[(static_cast<int>(size_) - kStarMin) / kStarDifference].x);
	trim.top = static_cast<long>(kStarInformation[(static_cast<int>(size_) - kStarMin) / kStarDifference].y) +
		id_ * static_cast<long>(kStarInformation[(static_cast<int>(size_) - kStarMin) / kStarDifference].z);
	trim.right = trim.left + static_cast<long>(kStarInformation[(static_cast<int>(size_) - kStarMin) / kStarDifference].z);
	trim.bottom = trim.top + static_cast<long>(kStarInformation[(static_cast<int>(size_) - kStarMin) / kStarDifference].z);

	Sprite::getInstance()->reserveDraw(overlay_texture_, position_, trim, 1.0F, 0.7F, Vector2(1.0F, 1.0F), -(XMConvertToRadians(angle_[0]) - XM_PIDIV2), Vector2(kStarInformation[(static_cast<int>(size_) - kStarMin) / kStarDifference].z / 2.0F, kStarInformation[(static_cast<int>(size_) - kStarMin) / kStarDifference].z / 2.0F), Sprite::getInstance()->chengeMode());
	Sprite::getInstance()->reserveDraw(texture_, position_, trim, 1.0F, 0.75F, Vector2(1.0F, 1.0F), -(XMConvertToRadians(angle_[0]) - XM_PIDIV2), Vector2(kStarInformation[(static_cast<int>(size_) - kStarMin) / kStarDifference].z / 2.0F, kStarInformation[(static_cast<int>(size_) - kStarMin) / kStarDifference].z / 2.0F));

	//軌跡の描画
	band_.draw(position_);
}

//生存確認
bool Star::isAlive()
{
	return alive_flag_;
}

//落下の実装
void Star::setFall()
{
	fall_ = kFallSpeed;
}

//当たり判定
void Star::collision(Player* P)
{
	float old_angle = Calc::angle(old_player_position_ - position_);
	float new_angle = Calc::angle(P->getPosition() - position_);

	//360度に変更
	if (old_angle < 0.0F) old_angle += XM_2PI;
	if (new_angle < 0.0F) new_angle += XM_2PI;

	//0度をまたいでいる場合360度を加算
	if (std::abs(new_angle - old_angle) > XM_PI)
		new_angle < old_angle ? new_angle += XM_2PI : old_angle += XM_2PI;

	//中心からの割合
	float per = ((P->getPosition() - position_).Length() / size_);
	//プレイヤーの移動量を取り出す
	float p_movement = (P->getMove()->end - P->getMove()->start).Length();


	spin_ += std::copysign(rate_ * per * p_movement, old_angle - new_angle);

	if (std::abs(spin_) < kMinSpin)
		spin_ = static_cast<float>(std::copysign(kMinSpin, spin_));
	if (std::abs(spin_) > kMaxSpin[id_])
		spin_ = static_cast<float>(std::copysign(kMaxSpin[id_], spin_));

	particle_time_ = 0;
	spining_angle_ = 0.0F;
}

void Star::collision(AIDemo * P)
{
	float old_angle = Calc::angle(old_player_position_ - position_);
	float new_angle = Calc::angle(P->getPosition() - position_);

	//360度に変更
	if (old_angle < 0.0F) old_angle += XM_2PI;
	if (new_angle < 0.0F) new_angle += XM_2PI;

	//0度をまたいでいる場合360度を加算
	if (std::abs(new_angle - old_angle) > XM_PI)
		new_angle < old_angle ? new_angle += XM_2PI : old_angle += XM_2PI;

	//中心からの割合
	float per = ((P->getPosition() - position_).Length() / size_);
	//プレイヤーの移動量を取り出す
	float p_movement = (P->getMove()->end - P->getMove()->start).Length();

	spin_ += std::copysign(rate_ * per * p_movement, new_angle - old_angle);

	if (std::abs(spin_) < kMinSpin)
		spin_ = static_cast<float>(std::copysign(kMinSpin, spin_));
	if (std::abs(spin_) > kMaxSpin[id_])
		spin_ = static_cast<float>(std::copysign(kMaxSpin[id_], spin_));

	particle_time_ = 0;
}

void Star::collision(FailWall * FW)
{
	alive_flag_ = false;
}

//落下のパーティクル生成
void Star::addFreeFallParticle()
{
	if (++particle_time_ >= kParticleTime)
	{
		if (fall_ >= 1.0F)
		{
			Vector2 create_position = position_ + Vector2(std::cos(XMConvertToRadians(spining_angle_)), -std::sin(XMConvertToRadians(spining_angle_))) * (size_ / 5 * 2.0F);
			spining_angle_ += std::copysign(360.0F / shape_num_ + 2.5F, spin_);
			//1フレームに1つの頂点から生成
			s_particle_container_.get()->addParticle(create_position, id_, 100);
			particle_time_ = 0;
		}
	}
}

float Star::getRotateDia()
{
	return 1.0F + (std::abs(spin_) - kMinSpin) / 10.0F;
}

//角度の設定
void Star::setAngle()
{
	//他の角度の指定
	for (int i = 1; i < angle_.size(); i++)
		angle_[i] = angle_[i - 1] - (360.0F / angle_.size());

	std::vector<Vector2> point(angle_.size());
	//頂点
	for (int i = 0; i < angle_.size(); ++i)
	{
		float size = (i % 2 == 0) ? size_ : (size_ * kStarVertexLengthDeff);
		point[i] = position_ + Vector2(std::cos(XMConvertToRadians(angle_[i])), -std::sin(XMConvertToRadians(angle_[i])))
			* size;
	}

	//始点と終点の設定
	for (int i = 1; i < point.size(); ++i)
		myshape_[i].start = myshape_[i - 1].end = point[i];

	myshape_[myshape_.size() - 1].end = myshape_[0].start = point[0];
}
