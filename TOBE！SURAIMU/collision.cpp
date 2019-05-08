#include "collision.h"
#include "calc.h"


Collision::Collision()
{}


Collision::~Collision()
{}

//----------------------------------------------------------------------------------------
//外部公開関数




//外部公開関数 End
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
//Privete関数

//円と円の当たり判定
bool Collision::judgment( const Circle & C1, const Circle & C2)
{
	//2点間の距離が2つの円の半径より小さい場合HIT
	if( Calc::magnitude( C1.position , C2.position ) < ( C1.radius + C2.radius ) )
		return true;

	return false;
}

//円と線の当たり判定
bool Collision::judgment( const Circle & C, const Line & L)
{
	//円と辺の始点と終点の判定
	if( judgment( C , Circle( L.start , 0.0F )) ||
				  judgment( C , Circle( L.end , 0.0F ) ))
				  return true;

	//円と線分の判定
	//判定用のベクトルを生成
	Vector2 checker[ 2 ] =
	{
		C.position - L.start,
		L.end - L.start
	};

	float innr = Calc::inner( checker[ 0 ] , checker[ 1 ] );
	float k = ( innr / Calc::magnitude( checker[ 1 ] ) ) / Calc::magnitude( checker[ 1 ] );

	if( k > 1.0F || k < 0.0F )
		return false;

	Vector2 per = ( checker[ 1 ] * k ) - checker[ 0 ];

	if( Calc::magnitude( per ) < C.radius )
		return true;

	return false;
}

//線と線の当たり判定
bool Collision::judgment( const Line & L1, const Line & L2)
{
	//対象が判定範囲内かどうかのチェック
	Vector2 check_start[ 3 ]
	{
		L2.end - L1.start,
		L2.start - L1.start,
		L1.end - L1.start
	};

	Vector2 check_end[ 3 ]
	{
		L2.end - L1.end,
		L2.start - L1.end,
		L1.start - L1.end
	};

	if( Calc::cross( check_start[ 2 ] , check_start[ 0 ] ) *Calc::cross( check_start[ 2 ] , check_start[ 1 ] ) > 0 &&
		Calc::cross( check_end[ 2 ] , check_end[ 0 ] ) *Calc::cross( check_end[ 2 ] , check_end[ 1 ] ) > 0 )
		return false;

	//対象と交点が存在するかチェック
	Vector2 check[ 3 ]
	{
		L1.start - L2.start,
		L1.end - L2.start,
		L2.end - L2.start
	};

	if( Calc::cross( check[ 2 ] , check[ 0 ] ) * Calc::cross( check[ 2 ] , check[ 1 ] ) < 0 )
		return true;

	return false;
}

//Privete関数 End
//----------------------------------------------------------------------------------------