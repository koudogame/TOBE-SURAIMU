#include "scoring.h"
#include "textureLoder.h"
#include "sprite.h"
#include "Sound.h"

const unsigned int kComboScore = 100;
const unsigned int kDownScore = 1;
const unsigned int kTechniqueScore = 1000;
const unsigned int kRotationScore = 100;
const unsigned int kLengthScore = 100;
const unsigned int kHeightScore = 10;
const unsigned int kLevelScore = 5000;
const int kNumWidth = 20;
const int kNumHeight = 32;
const int kMinNumWidth = 11;
const int kMinNumHeight = 20;
const float kScoreHeight = 50.0F;
const Vector2 kBasePosition = Vector2( 10.0F , 20.0F );
const Vector2 kTotalPosition = Vector2( 225.0F , 32.0F );
const RECT kBaseTrim = { 0 , 0 , 300 , 256 };

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
	add_num_texture_[kScore] = TextureLoder::getInstance()->load(L"Texture/Rank_number.png");
	//add_num_texture_[kCombo] = TextureLoder::getInstance()->load( L"Texture/" );
	//add_num_texture_[kHeight] = TextureLoder::getInstance()->load( L"Texture/" );

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

	return true;
}

void Scoring::update()
{
	bool delete_flag_ = false;
	int itr_num = 0;
	for( const auto& itr : addition_list_ )
	{
		//マジックナンバー
		itr.get()->update( 200.0F + itr_num * kMinNumHeight );

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
	RECT trim = kBaseTrim;
	Sprite::getInstance()->reserveDraw(texture_, kBasePosition, trim, 1.0F, 29.0F);

	//トータルスコアの描画
	unsigned long long temp = score_;
	Vector2 draw_position = kTotalPosition;
	do
	{
		trim.left = kNumWidth * ( temp % 10 );
		trim.right = trim.left + kNumWidth;
		trim.bottom = trim.top + kNumHeight;
		Sprite::getInstance()->reserveDraw( num_texture_ , draw_position , trim , 1.0F , 30.0F );
		draw_position.x -= kNumWidth;
		temp /= 10;
	} while( temp > 0ULL );

	//高さの描画
	draw_position.x = kTotalPosition.x;
	draw_position.y += kScoreHeight;
	unsigned int temp_ = static_cast< unsigned int >( height_ );
	do
	{
		trim.left = kNumWidth * ( temp_ % 10 );
		trim.right = trim.left + kNumWidth;
		trim.bottom = trim.top + kNumHeight;
		Sprite::getInstance()->reserveDraw( num_texture_ , draw_position , trim , 1.0F , 30.0F );
		draw_position.x -= kNumWidth;
		temp_ /= 10;
	} while( temp_ > 0ULL );

	//コンボの描画
	draw_position.x = kTotalPosition.x;
	draw_position.y += kScoreHeight;
	temp_ = combo_;
	do
	{
		trim.left = kNumWidth * ( temp_ % 10 );
		trim.right = trim.left + kNumWidth;
		trim.bottom = trim.top + kNumHeight;
		Sprite::getInstance()->reserveDraw( num_texture_ , draw_position , trim , 1.0F , 30.0F );
		draw_position.x -= kNumWidth;
		temp_ /= 10;
	} while( temp_ > 0ULL );


	//加点の描画
	for( const auto& itr : addition_list_ )
		itr.get()->draw();
}

//破棄
void Scoring::destroy()
{
	TextureLoder::getInstance()->release( texture_ );
	TextureLoder::getInstance()->release( num_texture_ );
	TextureLoder::getInstance()->release( add_num_texture_[kScore] );
	//TextureLoder::getInstance()->release( add_num_texture_[ kCombo ] );
	//TextureLoder::getInstance()->release( add_num_texture_[ kHeight ] );
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
			if( all_height_ >= 1.0F )
			{
				createNumber( static_cast< unsigned int >( all_height_ ) *  kHeightScore * level_ , add_num_texture_[ kScore ] );
				score_ += static_cast< unsigned long long >( all_height_ ) * static_cast< unsigned long long >( kHeightScore ) * static_cast< unsigned long long >( level_ );
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
		score_ += static_cast< unsigned long long >( combo_ ) * static_cast< unsigned long long >( kComboScore )* static_cast< unsigned long long >( level_ );
		createNumber( combo_ * kComboScore * level_ , add_num_texture_[ kScore ] );
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
		score_ += static_cast< unsigned long long >( kTechniqueScore ) * static_cast< unsigned long long >( ++technique_combo_ )* static_cast< unsigned long long >( level_ );
		createNumber( kTechniqueScore * technique_combo_ * level_ , add_num_texture_[ kScore ] );
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
		score_ += static_cast< unsigned long long >( kDownScore ) * static_cast< unsigned long long >( level_ );
}

//回転角加算
void Scoring::addRotate( float Angle )
{
	if( scoring_flag_ )
	{
		rotation_ += Angle;

		//1回転で加点
		if( XMConvertToRadians(rotation_) >= XM_2PI )
		{
			SOUND->stop( SoundId::kSpinCombo );
			SOUND->setPitch( SoundId::kSpinCombo , spin_combo_pitch_ );
			SOUND->play( SoundId::kSpinCombo , false );
			spin_combo_pitch_ += 0.2F;

			if( spin_combo_pitch_ > 1.0 )
				spin_combo_pitch_ = 1.0F;
			rotation_ = 0;
			rotation_combo_++;
			score_ += static_cast< unsigned long long >( rotation_combo_ ) * static_cast< unsigned long long >( kRotationScore )* static_cast< unsigned long long >( level_ );
			createNumber( rotation_combo_ * kRotationScore * level_ , add_num_texture_[ kScore ] );
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
void Scoring::addLength( const float Length )
{
	score_ += static_cast< unsigned long long >( static_cast< double >( Length ) * kLengthScore * level_ );
	createNumber( static_cast< unsigned int >( Length * kLengthScore * level_ ) , add_num_texture_[ kScore ] );
}

void Scoring::addLevel()
{
	level_++;
	SOUND->stop( SoundId::kLevelUp );
	SOUND->play( SoundId::kLevelUp , false );

	score_ += static_cast< unsigned long long >( kLevelScore ) * static_cast< unsigned long long >( level_ );
	createNumber( kLevelScore * level_ , add_num_texture_[ kScore ] );
}

void Scoring::createNumber( unsigned int Num ,ID3D11ShaderResourceView* Handle)
{
	addition_list_.push_front( std::make_shared<ScoreNumber>() );
	addition_list_.front().get()->init( Num , Handle );
}

