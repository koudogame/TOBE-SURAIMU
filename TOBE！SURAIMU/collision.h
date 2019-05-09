#pragma once
#include "shape.h"

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
//ŠO•”ŒöŠJŠÖ”
	bool collision( const Player& , const Star& );
	bool collision( const Player& , const Wall& );

private:
	bool judgment( const Circle& , const Circle& );
	bool judgment( const Circle& , const Line& );
	bool judgment( const Line& , const Line& );
};

