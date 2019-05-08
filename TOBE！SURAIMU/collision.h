#pragma once

class Player;
class Star;
class Wall;

class Collision
{
public:
	Collision();
	~Collision();

public:
//============================
//外部公開関数
	bool collision( const Player& , const Star& );
	bool collision( const Player& , const Wall& );



private:
//============================
//内部利用予定構造体
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
};

private:
	bool judgment( const Circle& , const Circle& );
	bool judgment( const Circle& , const Line& );
	bool judgment( const Line& , const Line& );
};

