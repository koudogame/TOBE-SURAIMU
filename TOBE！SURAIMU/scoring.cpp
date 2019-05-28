#include "scoring.h"
#include "textureLoder.h"
#include "sprite.h"

const unsigned int kComboScore = 100;
const unsigned int kDownScore = 1;
const unsigned int kTechniqueScore = 1000;
const unsigned int kRotationScore = 100;
const unsigned int kLengthScore = 100;
const unsigned int kHeightScore = 10;
const int kNumWidth = 20;
const int kNumHeight = 32;
const int kMinNumWidth = 11;
const int kMinNumHeight = 20;

Scoring::Scoring()
{}

Scoring::~Scoring()
{}

//=====================================================
//外部公開関数( 必ず実行 )

//初期化
bool Scoring::init()
{
	texture_ = TextureLoder::getInstance()->load( L"Texture/totalscore_left.png" );
	num_texture_ = TextureLoder::getInstance()->load( L"Texture/result_score.png" );
	score_ = 0;
	combo_ = 0;
	max_combo_ = 0;
	rotation_ = 0.0F;
	rotation_combo_ = 0;
	technique_combo_ = 0;
	spin_combo_pitch_ = 0.0F;
	height_ = 0.0;
	all_height_ = 0.0F;
	scoring_flag_ = false;

	combo_sound_ = AudioLoader::getInstance()->getSound( L"Sound/spincombo-dova.wav" );
	return true;
}

void Scoring::update()
{
	bool delete_flag_ = false;
	int itr_num = 0;
	for( const auto& itr : addition_list_ )
	{
		//マジックナンバー
		itr.get()->update( 84.0F + itr_num * kMinNumHeight );

		if( !itr.get()->isAlive() )
			delete_flag_ = true;

		itr_num++;
	}

	if( delete_flag_ )
		addition_list_.pop_back();
}

//描画
void Scoring::draw()
{
	//トータルスコアの下地描画
	//マジックナンバー
	RECT trim{ 0,0,300,64 };
	Sprite::getInstance()->draw( texture_ , Vector2(10.0F,20.0F) , &trim , 1.0F , 0.9F );
	//トータルスコアの描画
	Vector2 draw_position( 225.0F , 32.0F );
	unsigned long long temp = score_;
	do
	{
		trim.left = kNumWidth * ( temp % 10 );
		trim.right = trim.left + kNumWidth;
		trim.bottom = trim.top + kNumHeight;
		Sprite::getInstance()->draw( num_texture_ , draw_position , &trim , 1.0F , 1.0F );
		draw_position.x -= kNumWidth;
		temp /= 10;
	} while( temp > 0ULL );

	//加点の描画
	for( const auto& itr : addition_list_ )
		itr.get()->draw();
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
		if( AddScore > 0.0F )
		{
			height_ += AddScore;
			all_height_ += AddScore;
		}
		else
		{
			if( all_height_ > 0.0F )
			{
				createNumber( static_cast< unsigned int >( all_height_ ) *  kHeightScore );
				score_ += static_cast< unsigned long long >( all_height_ ) * static_cast< unsigned long long >( kHeightScore );
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
	if( scoring_flag_ )
	{
		combo_++;
		score_ += static_cast< unsigned long long >( combo_ ) * static_cast< unsigned long long >( kComboScore );
		createNumber( combo_ * kComboScore );
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
	{
		score_ += static_cast< unsigned long long >( kTechniqueScore ) * static_cast< unsigned long long >( ++technique_combo_ );
		createNumber( kTechniqueScore * technique_combo_ );
	}
}

void Scoring::resettechnique()
{
	technique_combo_ = 0;
}

//下降点加算
void Scoring::addDown()
{
	if( scoring_flag_ )
		score_ += static_cast< unsigned long long >( kDownScore );
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
			score_ += static_cast< unsigned long long >( rotation_combo_ ) * static_cast< unsigned long long >( kRotationScore );
			createNumber( rotation_combo_ * kRotationScore );
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
	score_ += static_cast< unsigned long long >( static_cast< double >( Length ) * kLengthScore );
	createNumber( static_cast< unsigned int >( Length * kLengthScore ) );
}

void Scoring::createNumber( unsigned int Num )
{
	addition_list_.push_front( std::make_shared<ScoreNumber>() );
	addition_list_.front().get()->init( Num );
}

