#include "score_number.h"
#include "textureLoder.h"
#include "sprite.h"

const int kNumWidth = 11;
const int kNumHeight = 15;
const float kSpeed = 6.0F;

ScoreNumber::ScoreNumber()
{
	texture_ = TextureLoder::getInstance()->load( L"Texture/Rank_number.png" );
	puluse_ = TextureLoder::getInstance()->load( L"Texture/totalscore_left.png" );
	alpha_ = 1.0F;
	position_ = Vector2( -kNumWidth , 0.0F );
}

ScoreNumber::~ScoreNumber()
{}

bool ScoreNumber::init( unsigned int Num )
{
	number_ = Num;
	return true;
}

void ScoreNumber::update( const float DrawPositY )
{
	position_.x += kSpeed;

	if( position_.x > 300.0F - kNumWidth )
	{
		position_.x = 300.0F - kNumWidth;
		alpha_ -= 0.01F;
	}

	position_.y = DrawPositY;
}

void ScoreNumber::draw()
{
	//”’l‚Ì•`‰æ
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

	//+‚Ì•`‰æ
	trim.left = 302;
	trim.right = trim.left + kNumWidth;
	Sprite::getInstance()->draw( puluse_ , draw_position , &trim , alpha_ , 1.0F );
}

bool ScoreNumber::isAlive()
{
	if( alpha_ < 0.0F )
		return false;

	return true;
}
