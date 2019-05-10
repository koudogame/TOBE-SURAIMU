#pragma once
#include "shape.h"

class Player;
class Star;
class Wall;

class Collision
{
public:
	~Collision();
private:
	//singleton
	Collision();

public:
//============================
//ŠO•”ŒöŠJŠÖ”
	static Collision* getInstance() { static Collision instance; return &instance; }
	void collision( Player* , Star* );
	void collision( Player* , Wall* );

private:
	//“–‚½‚è”»’è
	bool judgment( const Circle& , const Circle& );
	bool judgment( const Circle& , const Line& );
	bool judgment( const Line& , const Line& );

	//Œğ“_‚ğ‹‚ß‚é
	Vector2 crossPoint( const Circle& , const Line& );
	Vector2 crossPoint( const Line& , const Line& );
};

