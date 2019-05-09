#pragma once
//=======================================
//å`èÛç\ë¢ëÃ
struct Circle
{
	Vector2 position;
	float radius;

	Circle() = default;
	Circle( const Vector2& V , const float Rad ) { position = V; radius = Rad; }
	Circle( const float X , const float Y , const float Rad ) { position = Vector2( X , Y ); radius = Rad; }
};

struct Line
{
	Vector2 start;
	Vector2 end;
	Line() = default;
	Line( const Vector2& V1 , const Vector2& V2 ) { start = V1; end = V2; }
	Line( const float X1 , const float Y1 , const float X2 , const float Y2 ) { start = Vector2( X1 , Y1 ); end = Vector2( X2 , Y2 ); }
};