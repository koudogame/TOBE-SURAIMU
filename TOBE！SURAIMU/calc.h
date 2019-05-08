#pragma once

//計算用名前空間
namespace Calc
{
	// ベクトルの内積
	inline float inner( const Vector2& V , const Vector2& U )
	{
		return ( ( V.x * U.x ) + ( V.y * U.y ) );
	}

	//ベクトルの外積
	inline float cross( const Vector2& V1 , const Vector2& V2 )
	{
		return ( ( V1.x * V2.y ) - ( V1.y * V2.x ) );
	}

	// ベクトルの大きさ
	inline float magnitude( const Vector2& V )
	{
		return std::hypot( V.x , V.y );
	}

	// 2点間の距離
	inline float magnitude( const Vector2& V , const Vector2& U )
	{
		return std::hypot( V.x - U.x , V.y - U.y );
	}

	// ベクトルの正規化
	inline Vector2 norm( const Vector2& V )
	{
		return V / magnitude( V );
	}

	// ベクトルの角度
	inline float angle( const Vector2& V )
	{
		return std::atan2( -V.y , V.x );
	}

	// 2点間の角度
	inline float angle( const Vector2& V , const Vector2& U )
	{
		float magu_mal = magnitude( V ) * magnitude( U );
		float inner = Calc::inner( V , U );


		return std::acos( inner / magu_mal );
	}
}