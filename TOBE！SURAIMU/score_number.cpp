#include "score_number.h"
#include "textureLoder.h"
#include "sprite.h"

const int kNumWidth = 11;
const int kNumHeight = 15;
const float kSpeed = 6.0F;
const float kLimitLine = 300.0F;
const float kDffAlpha = 0.005F;
const int kPulseLeft = 302;

ScoreNumber::ScoreNumber()
{
}

ScoreNumber::~ScoreNumber()
{}

//初期化
bool ScoreNumber::init( unsigned int Num , ID3D11ShaderResourceView* Handle )
{
	puluse_ = TextureLoder::getInstance()->load( L"Texture/totalscore_left.png" );
	alpha_ = 1.0F;
	position_ = Vector2( -kNumWidth , 0.0F );
	number_ = Num;
	texture_ = Handle;
	return true;
}

//更新
void ScoreNumber::update( const float DrawPositY )
{
	//左に移動
	position_.x += kSpeed;

	//指定座標に行ったらアルファ値減少
	if( position_.x > kLimitLine - kNumWidth )
	{
		position_.x = kLimitLine - kNumWidth;
		alpha_ -= kDffAlpha;
	}

	position_.y = DrawPositY;
}

//描画
void ScoreNumber::draw()
{
	//数値の描画
	RECT trim = { 0,0,0,0 };
	Vector2 draw_position = position_;
	for( unsigned long long temp = number_; temp != 0; temp /= 10 )
	{
		trim.left = kNumWidth * ( temp % 10 );
		trim.right = trim.left + kNumWidth;
		trim.bottom = trim.top + kNumHeight;
		Sprite::getInstance()->draw( texture_ , draw_position , &trim , alpha_ , 1.0F );
		draw_position.x -= kNumWidth;
	}

	//+の描画
	trim.left = kPulseLeft;
	trim.right = trim.left + kNumWidth;
	Sprite::getInstance()->draw( puluse_ , draw_position , &trim , alpha_ , 1.0F );
}

//生存確認
bool ScoreNumber::isAlive()
{
	if( alpha_ < 0.0F )
		return false;

	return true;
}
