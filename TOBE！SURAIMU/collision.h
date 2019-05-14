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
	bool judgment( Circle* , Circle* );
	bool judgment( Circle* , Line* );
	bool judgment( Line* , Line* );

	//Œğ“_‚ğ‹‚ß‚é
	Vector2 crossPoint( Circle* , Line* );
	Vector2 crossPoint( Line* , Line* );
};

