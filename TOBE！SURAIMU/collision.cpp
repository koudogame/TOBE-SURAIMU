#include "collision.h"
#include "calc.h"

//判定対象
#include "player.h"
#include "star.h"
#include "wall.h"

Collision::Collision()
{}


Collision::~Collision()
{}

//----------------------------------------------------------------------------------------
//外部公開関数

void Collision::collision( Player * P , Star * S )
{
	bool hit_flag = false;
	for( int i = 0; i < kStarLineNum; i++ )
	{
		//円と辺の始点と終点の判定
		if( judgment( P->getShape() , &Circle( S->getShape( i )->start , 0.0F ) ) )
		{
			if( !P->isCollision() )
			{
				return;
			}
		}
	}

	for( int i = 0; i < kStarLineNum; i++ )
	{
		//円と線の当たり判定
		if( judgment( P->getShape() , S->getShape( i ) ) )
		{
			if( P->getOwner() != S )
			{
				P->setGround( S->getShape( i ) );
				P->revision( crossPoint( P->getShape() , S->getShape( i ) ) );
				hit_flag = true;
			}
			else
			{
				if( !P->isCollision() )
				{
					P->setGround( S->getShape( i ) );
					P->revision( crossPoint( P->getShape() , S->getShape( i ) ) );
					hit_flag = true;
				}

			}
		}
		else if( judgment( P->getMove() , S->getShape( i ) ) )
		{
			if( P->getOwner() != S )
			{
				P->setGround( S->getShape( i ) );
				P->revision( crossPoint( P->getShape() , S->getShape( i ) ) );
					hit_flag = true;
			}
			else
			{
				if( !P->isCollision() )
				{
					P->setGround( S->getShape( i ) );
					P->revision( crossPoint( P->getShape() , S->getShape( i ) ) );
					hit_flag = true;
				}

			}
		}
	}

	if( hit_flag )
	{
		if( P->getOwner() != S )
		{
			P->collision( S );
			S->collision( P );
		}
		else
		{
			if( !P->isCollision() )
			{
				P->collision( S );
			}
		}
	}
}

void Collision::collision( Player * P , Wall * W )
{
	for( int i = 0; i < 2; i++ )
	{
		if( judgment(P->getShape() , W->getShape( i ) ) || judgment( P->getMove() , W->getShape( i ) ))
		{
			//円と線の当たり判定
			P->setGround( W->getShape( i ) );
			P->revision( crossPoint( P->getShape() , W->getShape( i ) ) );
			P->collision( W );
			break;
		}
	}
}


//外部公開関数 End
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
//Privete関数

//---当たり判定---

//円と円の当たり判定
bool Collision::judgment(  Circle * C1,  Circle * C2)
{
	//2点間の距離が2つの円の半径より小さい場合HIT
	if( Calc::magnitude( C1->position , C2->position ) < ( C1->radius + C2->radius ) )
		return true;

	return false;
}

//円と線の当たり判定
bool Collision::judgment(  Circle * C,  Line * L)
{
	//円と線分の判定
	//判定用のベクトルを生成
	Vector2 checker[ 2 ] =
	{
		C->position - L->start,
		L->end - L->start
	};

	float innr = Calc::inner( checker[ 0 ] , checker[ 1 ] );
	float k = ( innr / Calc::magnitude( checker[ 1 ] ) ) / Calc::magnitude( checker[ 1 ] );

	if( k > 1.0F || k < 0.0F )
		return false;

	Vector2 per = ( checker[ 1 ] * k ) - checker[ 0 ];

	if( Calc::magnitude( per ) < C->radius )
		return true;

	return false;
}

//線と線の当たり判定
bool Collision::judgment(  Line * L1,  Line * L2)
{
	//対象が判定範囲内かどうかのチェック
	Vector2 check_start[ 3 ]
	{
		L2->end - L1->start,
		L2->start - L1->start,
		L1->end - L1->start
	};

	Vector2 check_end[ 3 ]
	{
		L2->end - L1->end,
		L2->start - L1->end,
		L1->start - L1->end
	};

	if( Calc::cross( check_start[ 2 ] , check_start[ 0 ] ) *Calc::cross( check_start[ 2 ] , check_start[ 1 ] ) > 0 &&
		Calc::cross( check_end[ 2 ] , check_end[ 0 ] ) *Calc::cross( check_end[ 2 ] , check_end[ 1 ] ) > 0 )
		return false;

	//対象と交点が存在するかチェック
	Vector2 check[ 3 ]
	{
		L1->start - L2->start,
		L1->end - L2->start,
		L2->end - L2->start
	};

	if( Calc::cross( check[ 2 ] , check[ 0 ] ) * Calc::cross( check[ 2 ] , check[ 1 ] ) < 0 )
		return true;

	return false;
}


//---交点を求める---

Vector2 Collision::crossPoint(  Circle * C ,  Line * L )
{
	//円と線分
	//判定用のベクトルを生成
	Vector2 checker[ 2 ] =
	{
		C->position - L->start,
		L->end - L->start
	};

	float innr = Calc::inner( checker[ 0 ] , checker[ 1 ] );
	float k = ( innr / Calc::magnitude( checker[ 1 ] ) ) / Calc::magnitude( checker[ 1 ] );

	return L->start + checker[ 1 ] * k;
}

Vector2 Collision::crossPoint(  Line * L1,  Line * L2)
{
	//分子
	float mol[ 2 ];
	//分母
	float deno[ 2 ];

	//xの変化量( 分母 )
	deno[ 0 ] = L1->end.x - L1->start.x;
	deno[ 1 ] = L2->end.x - L2->start.x;

	//yの変化量( 分子 )
	mol[0] = L1->end.y - L1->start.y;
	mol[1] = L2->end.y - L2->start.y;

	//xの変化量がない場合
	if( deno[ 0 ] == 0 )
	{
		float tilt = mol[ 1 ] / deno[ 1 ];

		float y = tilt * ( L1->start.x - L2->start.x ) + L2->start.y;

		return Vector2( L1->start.x , y );
	}
	else if( deno[ 1 ] == 0 )
	{
		float tilt = mol[ 0 ] / deno[ 0 ];

		float y = tilt * ( L2->start.x - L1->start.x ) + L1->start.y;

		return Vector2( L2->start.x , y );
	}

	//交点を求める
	float tilt[ 2 ] = {
		mol[ 0 ] / deno[ 0 ],
		mol[ 1 ] / deno[ 1 ]
	};

	Vector2 cross_point;

	cross_point.x = ( tilt[ 1 ] * L2->start.x - L2->start.y - tilt[ 0 ] * L1->start.x + L1->start.y ) / ( tilt[ 1 ] - tilt[ 0 ] );
	cross_point.y = tilt[ 0 ] * ( cross_point.x - L1->start.x ) + L1->start.y;

	return cross_point;
}

//Privete関数 End
//----------------------------------------------------------------------------------------