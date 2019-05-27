#include "scoring.h"
#include "textureLoder.h"

const int kComboScore = 100;
const int kDownScore = 1;
const int kTechniqueScore = 1000;
const int kRotationScore = 100;
const int kLengthScore = 100;

Scoring::Scoring()
{}

Scoring::~Scoring()
{}

//=====================================================
//外部公開関数( 必ず実行 )

//初期化
bool Scoring::init()
{
	texture_ = TextureLoder::getInstance()->load( L"Texture/数字.png" );
	score_ = 0;
	combo_ = 0;
	max_combo_ = 0;
	rotation_ = 0.0F;
	rotation_combo_ = 0;
	spin_combo_pitch_ = 0.0F;
	scoring_flag_ = false;

	combo_sound_ = AudioLoader::getInstance()->getSound( L"Sound/spincombo-dova.wav" );
	return true;
}

//描画
void Scoring::draw()
{
	sco_ = score_;
	move_com_ = combo_;
	rota_com_ = rotation_combo_;

	sco_.draw( texture_ , Vector2( 320.0F , 0.0F ) , 64 , 128 );
	move_com_.draw( texture_ , Vector2( 320.0F , 128.0F ) , 64 , 128 );
	rota_com_.draw( texture_ , Vector2( 320.0F , 256.0F ) , 64 , 128 );
}

//破棄
void Scoring::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
}

//上昇量加算
void Scoring::addDefaultScore( const double AddScore )
{
	if( scoring_flag_ )
	{
		height_ += AddScore;
		score_ += static_cast< int >( AddScore );
	}
}


//=====================================================
//外部公開関数

//移動コンボの増加
void Scoring::addCombo()
{
	if( scoring_flag_ )
	{
		combo_++;

		score_ += combo_ * kComboScore;
	}

	if( combo_ > max_combo_ )
		max_combo_ = combo_;
}

//移動コンボのリセット
void Scoring::resetCombo()
{
	combo_ = 0;
}

//テクニック点加算
void Scoring::addTechnique()
{
	if( scoring_flag_ )
		score_ += kTechniqueScore;
}

//下降点加算
void Scoring::addDown()
{
	if( scoring_flag_ )
		score_ += kDownScore;
}

//回転角加算
void Scoring::addRotate( float Angle )
{
	if( scoring_flag_ )
	{
		rotation_ += Angle;

		//1回転で加点
		if( rotation_ >= 360.0F )
		{
			combo_sound_->stop();
			combo_sound_->play( AudioContainer::Mode::kDefault );
			spin_combo_pitch_ += 0.2F;
			if( spin_combo_pitch_ > 1.0 )
				spin_combo_pitch_ = 1.0F;
			combo_sound_->setPitch( spin_combo_pitch_ );
			rotation_ = 0;
			rotation_combo_++;
			score_ += rotation_combo_ * kRotationScore;
		}
	}
}

//回転角リセット
void Scoring::resetRotate()
{
	combo_sound_->resetPitch();
	spin_combo_pitch_ = 0.0F;
	rotation_ = 0;
	rotation_combo_ = 0;
}

//星の距離で加点
void Scoring::addLength( const float Length )
{
	score_ += static_cast< int >( Length * kLengthScore );
}

