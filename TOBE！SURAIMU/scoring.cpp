#include "scoring.h"
#include "textureLoder.h"
#include "sprite.h"
#include "Sound.h"

//加点
const unsigned int kComboScore = 100;
const unsigned int kDownScore = 1;
const unsigned int kTechniqueScore = 1000;
const unsigned int kRotationScore = 100;
const unsigned int kLengthScore = 100;
const unsigned int kHeightScore = 100;
const unsigned int kLevelScore = 50000;
//サイズ
const int kNumWidth = 20;
const int kNumHeight = 32;
const int kMinNumWidth = 10;
const int kMinNumHeight = 14;
const float kScoreHeight = 50.0F;
const float kAddScoreCreateLine = 175.0F;
const float kIntervalHeight = 2.0F;
//位置
const Vector2 kBasePosition = Vector2(-23.0F, 20.0F);
const Vector2 kTotalPosition = Vector2(190.0F, 32.0F);
const Vector2 kComboCircleAnker = Vector2(160 / 2.0F, 160 / 2.0F);
const Vector2 kComboSpriteAnker = Vector2(96 / 2.0F, 38 / 2.0F + 20.0F);
//切り取り
const RECT kBaseTrim = { 0 , 0 , 300 , 256 };
const RECT kComboCircleTrim = { 0,0,160,160 };
const RECT kComboSpriteTrim = { kComboCircleTrim.right,kComboCircleTrim.top,256,38 };
//定数
const int kComboResetMilliTime = 2000;
const float kAddComboScale = 0.2F;
const float kAddComboSpriteScale = 0.05F;
const float kDefComboScale = 1.0F / (kComboResetMilliTime / 1000.0F) / (60 - (1.0F / kAddComboScale) / (kComboResetMilliTime / 1000.0F));
const float kComboSpriteMaxScale = 2.5F;
const float kComboAlphaDefThre = 1.0F;
const float kComnboSpriteAlphaDefVol = 1.0F / ((kComboSpriteMaxScale - kComboAlphaDefThre) / kAddComboSpriteScale);

Scoring::Scoring()
{}

Scoring::~Scoring()
{}

//=====================================================
//外部公開関数( 必ず実行 )
//初期化
bool Scoring::init()
{
	texture_ = TextureLoder::getInstance()->load(L"Texture/totalscore_left.png");
	num_texture_ = TextureLoder::getInstance()->load(L"Texture/result_score.png");
	combo_texture_ = TextureLoder::getInstance()->load(L"Texture/combo.png");
	add_num_texture_ = TextureLoder::getInstance()->load(L"Texture/ranking_number.png");

	score_ = 0;
	combo_ = 0;
	max_combo_ = 0;
	rotation_ = 0.0F;
	rotation_combo_ = 0;
	technique_combo_ = 0;
	spin_combo_pitch_ = 0.0F;
	height_ = 0.0;
	all_height_ = 0.0F;
	level_ = 1;
	scoring_flag_ = false;
	isexp_now_ = false;
	combo_circle_scale_ = 0.0F;
	combo_sprite_scale_ = 0.0F;
	combo_alpha_ = 1.0F;
	return true;
}

void Scoring::update()
{
	bool delete_flag_ = false;
	int itr_num = 0;
	for (const auto& itr : addition_list_)
	{
		itr.get()->update(kAddScoreCreateLine + itr_num * (kMinNumHeight + kIntervalHeight));

		if (!itr.get()->isAlive())
			delete_flag_ = true;

		itr_num++;
	}

	if (delete_flag_)
		addition_list_.pop_back();

	if (kComboResetMilliTime - combo_timer_.getCount() <= 0)
	{
		combo_ = 0;
		combo_circle_scale_ = 0.0F;
		combo_sprite_scale_ = 0.0F;
	}

	if (isexp_now_)
	{
		if (combo_circle_scale_ < 1.0F)
		{
			combo_circle_scale_ += kAddComboScale;
			combo_sprite_scale_ += kAddComboScale;
		}
		else
			isexp_now_ = false;
	}
	else
	{
		if (!player_jump_now_flag_)
		{
			if (combo_circle_scale_ > 0.0F)
			{
				combo_circle_scale_ -= kDefComboScale;
				if (combo_sprite_scale_ < kComboSpriteMaxScale && combo_sprite_scale_ > 0.0F)
				{
					combo_sprite_scale_ += kAddComboSpriteScale;
					if (combo_sprite_scale_ > kComboAlphaDefThre)
						combo_alpha_ -= kComnboSpriteAlphaDefVol;
				}
			}
			else
				combo_circle_scale_ = 0.0F;
		}
		else
		{
			combo_sprite_scale_ = 0.0F;
		}
	}
}

//描画
void Scoring::draw()
{
	//トータルスコアの下地描画
	RECT trim = kBaseTrim;
	Sprite::getInstance()->reserveDraw(texture_, kBasePosition, trim, 1.0F, 29.0F);

	//トータルスコアの描画
	unsigned long long temp = score_;
	Vector2 draw_position = kTotalPosition;
	do
	{
		trim.left = kNumWidth * (temp % 10);
		trim.right = trim.left + kNumWidth;
		trim.bottom = trim.top + kNumHeight;
		Sprite::getInstance()->reserveDraw(num_texture_, draw_position, trim, 1.0F, 30.0F);
		draw_position.x -= kNumWidth;
		temp /= 10;
	} while (temp > 0ULL);

	//高さの描画
	draw_position.x = kTotalPosition.x;
	draw_position.y += kScoreHeight;
	unsigned int temp_ = static_cast<unsigned int>(height_);
	do
	{
		trim.left = kNumWidth * (temp_ % 10);
		trim.right = trim.left + kNumWidth;
		trim.bottom = trim.top + kNumHeight;
		Sprite::getInstance()->reserveDraw(num_texture_, draw_position, trim, 1.0F, 30.0F);
		draw_position.x -= kNumWidth;
		temp_ /= 10;
	} while (temp_ > 0ULL);

	//コンボの描画
	draw_position.x = kTotalPosition.x;
	draw_position.y += kScoreHeight;
	temp_ = combo_;
	do
	{
		trim.left = kNumWidth * (temp_ % 10);
		trim.right = trim.left + kNumWidth;
		trim.bottom = trim.top + kNumHeight;
		Sprite::getInstance()->reserveDraw(num_texture_, draw_position, trim, 1.0F, 30.0F);
		draw_position.x -= kNumWidth;
		temp_ /= 10;
	} while (temp_ > 0ULL);

	//コンボエフェクトの描画
	if (!player_jump_now_flag_)
	{
		Sprite::getInstance()->reserveDraw(combo_texture_, player_position_, kComboCircleTrim, 0.6F, 0.76F, Vector2(combo_circle_scale_, combo_circle_scale_), 0.0F, kComboCircleAnker);
		Sprite::getInstance()->reserveDraw(combo_texture_, player_position_, kComboSpriteTrim, combo_alpha_, 0.76F, Vector2(combo_sprite_scale_, combo_sprite_scale_), 0.0F, kComboSpriteAnker);
	}

	//加点の描画
	for (const auto& itr : addition_list_)
		itr.get()->draw();
}

//破棄
void Scoring::destroy()
{
	TextureLoder::getInstance()->release(texture_);
	TextureLoder::getInstance()->release(num_texture_);
	TextureLoder::getInstance()->release(add_num_texture_);
	TextureLoder::getInstance()->release(combo_texture_);
}

//上昇量加算
void Scoring::addDefaultScore(const double AddScore)
{
	if (scoring_flag_)
	{
		if (AddScore > 0.0F)
		{
			height_ += AddScore;
			all_height_ += AddScore;
		}
		else
		{
			if (all_height_ >= 1.0F)
			{
				createNumber(static_cast<unsigned int>(all_height_) *  kHeightScore * level_, add_num_texture_);
				score_ += static_cast<unsigned long long>(all_height_) * static_cast<unsigned long long>(kHeightScore) * static_cast<unsigned long long>(level_);
			}
			all_height_ = 0.0F;
		}
	}
}


//=====================================================
//外部公開関数

//移動コンボの増加
void Scoring::addCombo()
{
	if (scoring_flag_)
	{
		combo_++;
		timeRestart();
		combo_timer_.start();
		score_ += static_cast<unsigned long long>(combo_) * static_cast<unsigned long long>(kComboScore)* static_cast<unsigned long long>(level_);
		createNumber(combo_ * kComboScore * level_, add_num_texture_);
		isexp_now_ = true;
		combo_circle_scale_ = 0.0F;
		combo_sprite_scale_ = 0.0F;
		combo_alpha_ = 1.0F;
	}

	if (combo_ > max_combo_)
		max_combo_ = combo_;
}
//テクニック点加算
void Scoring::addTechnique()
{
	if (scoring_flag_)
	{
		score_ += static_cast<unsigned long long>(kTechniqueScore) * static_cast<unsigned long long>(++technique_combo_)* static_cast<unsigned long long>(level_);
		createNumber(kTechniqueScore * technique_combo_ * level_, add_num_texture_);
	}
}

void Scoring::resettechnique()
{
	technique_combo_ = 0;
}

//下降点加算
void Scoring::addDown()
{
	if (scoring_flag_)
		score_ += static_cast<unsigned long long>(kDownScore) * static_cast<unsigned long long>(level_);
}

//回転角加算
void Scoring::addRotate(float Angle)
{
	if (scoring_flag_)
	{
		rotation_ += Angle;

		//1回転で加点
		if (XMConvertToRadians(rotation_) >= XM_2PI)
		{
			SOUND->stop(SoundId::kSpinCombo);
			SOUND->setPitch(SoundId::kSpinCombo, spin_combo_pitch_);
			SOUND->play(SoundId::kSpinCombo, false);
			spin_combo_pitch_ += 0.2F;

			if (spin_combo_pitch_ > 1.0)
				spin_combo_pitch_ = 1.0F;
			rotation_ = 0;
			rotation_combo_++;
			score_ += static_cast<unsigned long long>(rotation_combo_) * static_cast<unsigned long long>(kRotationScore)* static_cast<unsigned long long>(level_);
			createNumber(rotation_combo_ * kRotationScore * level_, add_num_texture_);
		}
	}
}

//回転角リセット
void Scoring::resetRotate()
{
	spin_combo_pitch_ = 0.0F;
	rotation_ = 0;
	rotation_combo_ = 0;
}

//星の距離で加点
void Scoring::addLength(const float Length)
{
	score_ += static_cast<unsigned long long>(static_cast<double>(Length) * kLengthScore * level_);
	createNumber(static_cast<unsigned int>(Length * kLengthScore * level_), add_num_texture_);
}

void Scoring::addLevel()
{
	level_++;
	SOUND->stop(SoundId::kLevelUp);
	SOUND->play(SoundId::kLevelUp, false);

	score_ += static_cast<unsigned long long>(kLevelScore) * static_cast<unsigned long long>(level_);
	createNumber(kLevelScore * level_, add_num_texture_);
}

void Scoring::createNumber(unsigned int Num, ID3D11ShaderResourceView* Handle)
{
	addition_list_.push_front(std::make_shared<ScoreNumber>());
	addition_list_.front().get()->init(Num, Handle);
}

