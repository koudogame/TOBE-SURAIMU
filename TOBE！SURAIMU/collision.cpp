#include "collision.h"
#include "calc.h"

Collision::Collision()
{
}


Collision::~Collision()
{
}

//----------------------------------------------------------------------------------------
//外部公開関数

void Collision::init()
{
	onece_flag_ = false;
	start_flag_ = false;
}

//当たり判定
bool Collision::collision(ObjectBase * Obj1, ObjectBase * Obj2)
{
	switch (Obj1->getID())
	{
	case ObjectID::kPlayer:
		switch (Obj2->getID())
		{
		case ObjectID::kPlayer:
			return collision(dynamic_cast<Player*>(Obj1), dynamic_cast<Player*>(Obj2));

		case ObjectID::kStar:
			return collision(dynamic_cast<Player*>(Obj1), dynamic_cast<Star*>(Obj2));

		case ObjectID::kWall:
			return collision(dynamic_cast<Player*>(Obj1), dynamic_cast<Wall*>(Obj2));

		case ObjectID::kFailWall:
			return collision(dynamic_cast<Player*>(Obj1), dynamic_cast<FailWall*>(Obj2));

		default:
			return false;
		}
		break;

	case ObjectID::kStar:
		switch (Obj2->getID())
		{
		case ObjectID::kPlayer:
			return collision(dynamic_cast<Player*>(Obj2), dynamic_cast<Star*>(Obj1));

		case ObjectID::kSerch:
			return collision(dynamic_cast<Sercher*>(Obj2), dynamic_cast<Star*>(Obj1));

		case ObjectID::kFailWall:
			return collision(dynamic_cast<Star*>(Obj1), dynamic_cast<FailWall*>(Obj2));

		default:
			return false;
		}
		break;

	case ObjectID::kWall:
		switch (Obj2->getID())
		{
		case ObjectID::kPlayer:
			return collision(dynamic_cast<Player*>(Obj2), dynamic_cast<Wall*>(Obj1));

		default:
			return false;
		}
		break;

	case ObjectID::kSerch:
		switch (Obj2->getID())
		{
		case ObjectID::kStar:
			return collision(dynamic_cast<Sercher*>(Obj1), dynamic_cast<Star*>(Obj2));
		}
		break;

	case ObjectID::kFailWall:
		switch (Obj2->getID())
		{
		case ObjectID::kPlayer:
			return collision(dynamic_cast<FailWall*>(Obj1), dynamic_cast<Player*>(Obj2));

		case ObjectID::kStar:
			return collision(dynamic_cast<FailWall*>(Obj1), dynamic_cast<Star*>(Obj2));

		default:
			return false;
		}
		break;


	default:
		return false;
	}

	return false;
}

//プレイヤー対星
bool Collision::collision(Player * P, Star * S)
{
	if (!P->isJump() && start_flag_)
		return true;

	id_ = S->getColor();

	bool hit_flag = false;
	for (int i = 0; i < kStarLineNum; i++)
	{
		//円と辺の始点との判定
		if (judgment(P->getShape(), &Circle(S->getShape(i)->start, 0.0F)) ||
			judgment(&Circle(S->getShape(i)->start, 0.0F), P->getMove()))
		{
			if (P->getOwner() != S)
			{
				P->setGround(S->getShape(i));
				P->revision(S->getShape(i)->start, id_);
				hit_flag = true;
			}
			else
			{
				if (!P->isCollision())
				{
					P->setGround(S->getShape(i));
					P->revision(S->getShape(i)->start, id_);
					hit_flag = true;
				}

			}
		}
	}

	for (int i = 0; i < kStarLineNum; i++)
	{
		//線と線の当たり判定
		if (judgment(P->getMove(), S->getShape(i)))
		{
			if (P->getOwner() != S)
			{
				P->setGround(S->getShape(i));
				P->revision(crossPoint(P->getMove(), S->getShape(i)), id_);
				hit_flag = true;
			}
			else
			{
				if (!P->isCollision())
				{
					P->setGround(S->getShape(i));
					P->revision(crossPoint(P->getMove(), S->getShape(i)), id_);
					hit_flag = true;
				}

			}
		}
		//円と線の当たり判定
		else if (judgment(P->getShape(), S->getShape(i)))
		{
			if (P->getOwner() != S)
			{
				P->setGround(S->getShape(i));
				P->revision(crossPoint(P->getShape(), S->getShape(i)), id_);
				hit_flag = true;
			}
			else
			{
				if (!P->isCollision())
				{
					P->setGround(S->getShape(i));
					P->revision(crossPoint(P->getShape(), S->getShape(i)), id_);
					hit_flag = true;
				}

			}
		}
	}

	//判定があったら
	if (hit_flag)
	{
		if (P->getOwner() != S)
		{
			S->collision(P);
			P->collision(S);
			onece_flag_ = true;
		}
		else
		{
			if (!P->isCollision())
			{
				if (!onece_flag_)
				{
					S->collision(P);
					onece_flag_ = true;
				}
				P->collision(S);
			}
		}

		start_flag_ = true;
		return true;
	}

	return false;
}

//プレイヤー対壁
bool Collision::collision(Player * P, Wall * W)
{
	if (!P->isJump())
		return false;

	for (int i = 0; i < 2; i++)
	{
		if (judgment(P->getShape(), W->getShape(i)))
		{
			if (!P->isWallCollision())
			{
				//円と線の当たり判定
				P->setGround(W->getShape(i));
				P->revision(crossPoint(P->getShape(), W->getShape(i)), NameSpaceParticle::ParticleID::kWall);
				P->collision(W);
				onece_flag_ = false;
			}

			return true;
		}
		else if (judgment(P->getMove(), W->getShape(i)))
		{
			//円と線の当たり判定
			if (!P->isWallCollision())
			{
				//円と線の当たり判定
				P->setGround(W->getShape(i));
				P->revision(crossPoint(P->getMove(), W->getShape(i)), NameSpaceParticle::ParticleID::kWall);
				P->collision(W);
				onece_flag_ = false;
			}
			return true;
		}
	}

	P->resetWallCollisionFlag();

	return false;
}

//プレイヤー対プレイヤー
bool Collision::collision(Player * P1, Player * P2)
{
	if (!P1->isJump() || !P2->isJump() || P1 == P2)
		return true;

	//円と円の当たり判定
	if (judgment(P1->getShape(), P2->getShape()))
	{
		if (!P1->isPlayerCollision() || !P2->isPlayerCollision())
		{
			P1->resetPlayerCollisionFlag();
			P2->resetPlayerCollisionFlag();
			P1->collision(P2);
			P2->collision(P1);
		}

		return true;
	}

	P1->resetPlayerCollisionFlag();
	P2->resetPlayerCollisionFlag();

	return false;
}

bool Collision::collision(Sercher * SRS, Star * S)
{
	for (int i = 0; i < kStarLineNum; i++)
		if (judgment(&SRS->getRange(), S->getShape(i)))
		{
			SRS->collision(S);
			return true;
		}

	return false;
}

bool Collision::collision(Player * P, FailWall * FW)
{
	Vector2 vect[2] = { FW->getShape()->end - FW->getShape()->start,
	P->getShape()->position - FW->getShape()->start };

	vect[0].Normalize();
	vect[1].Normalize();

	if (vect[0].x > vect[1].x)
		vect[1].x *= -1;

	if (Calc::cross(vect[0], vect[1]) > 0)
	{
		P->collision(FW);
		return true;
	}
	return false;
}

bool Collision::collision(Star * S, FailWall * FW)
{
	for (int i = 0; i < 5; i++)
	{
		Vector2 judge_vector[2] = { FW->getShape()->end - FW->getShape()->start,S->getShape(i)->start - FW->getShape()->start };
		judge_vector[0].Normalize();
		judge_vector[1].Normalize();
		if (Calc::cross(judge_vector[0], judge_vector[1]) < 0)
		{
			return false;
		}
	}
	S->collision(FW);
	return true;
}


//外部公開関数 End
//----------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------
//Privete関数

//---当たり判定---

//円と円の当たり判定
bool Collision::judgment(const Circle * const C1, const Circle * const C2)
{
	//2点間の距離が2つの円の半径より小さい場合HIT
	if (Calc::magnitude(C1->position, C2->position) < (C1->radius + C2->radius))
		return true;

	return false;
}

//円と線の当たり判定
bool Collision::judgment(const Circle * const C, const Line * const L)
{
	//円と線分の判定
	//判定用のベクトルを生成
	Vector2 checker[2] =
	{
		C->position - L->start,
		L->end - L->start
	};

	float innr = Calc::inner(checker[0], checker[1]);
	float k = (innr / Calc::magnitude(checker[1])) / Calc::magnitude(checker[1]);

	if (k > 1.0F || k < 0.0F)
		return false;

	Vector2 per = (checker[1] * k) - checker[0];

	if (Calc::magnitude(per) < C->radius)
		return true;

	return false;
}

//線と線の当たり判定
bool Collision::judgment(const Line * const L1, const Line * const L2)
{
	//対象が判定範囲内かどうかのチェック
	Vector2 check_start[3]
	{
		L2->end - L1->start,
		L2->start - L1->start,
		L1->end - L1->start
	};

	Vector2 check_end[3]
	{
		L2->end - L1->end,
		L2->start - L1->end,
		L1->start - L1->end
	};

	if (Calc::cross(check_start[2], check_start[0]) *Calc::cross(check_start[2], check_start[1]) > 0 &&
		Calc::cross(check_end[2], check_end[0]) *Calc::cross(check_end[2], check_end[1]) > 0)
		return false;

	//対象と交点が存在するかチェック
	Vector2 check[3]
	{
		L1->start - L2->start,
		L1->end - L2->start,
		L2->end - L2->start
	};

	if (Calc::cross(check[2], check[0]) * Calc::cross(check[2], check[1]) < 0)
		return true;

	return false;
}


//---交点を求める---

Vector2 Collision::crossPoint(const Circle * const C, const Line * const L)
{
	//円と線分
	//判定用のベクトルを生成
	Vector2 checker[2] =
	{
		C->position - L->start,
		L->end - L->start
	};

	float innr = Calc::inner(checker[0], checker[1]);
	float k = (innr / Calc::magnitude(checker[1])) / Calc::magnitude(checker[1]);

	return L->start + checker[1] * k;
}

Vector2 Collision::crossPoint(const Line * const L1, const Line * const L2)
{
	//分子
	float mol[2];
	//分母
	float deno[2];

	//xの変化量( 分母 )
	deno[0] = L1->end.x - L1->start.x;
	deno[1] = L2->end.x - L2->start.x;

	//yの変化量( 分子 )
	mol[0] = L1->end.y - L1->start.y;
	mol[1] = L2->end.y - L2->start.y;

	//xの変化量がない場合
	if (deno[0] == 0)
	{
		float tilt = mol[1] / deno[1];

		float y = tilt * (L1->start.x - L2->start.x) + L2->start.y;

		return Vector2(L1->start.x, y);
	}
	else if (deno[1] == 0)
	{
		float tilt = mol[0] / deno[0];

		float y = tilt * (L2->start.x - L1->start.x) + L1->start.y;

		return Vector2(L2->start.x, y);
	}

	//交点を求める
	float tilt[2] = {
		mol[0] / deno[0],
		mol[1] / deno[1]
	};

	Vector2 cross_point;

	cross_point.x = (tilt[1] * L2->start.x - L2->start.y - tilt[0] * L1->start.x + L1->start.y) / (tilt[1] - tilt[0]);
	cross_point.y = tilt[0] * (cross_point.x - L1->start.x) + L1->start.y;

	return cross_point;
}

//Privete関数 End
//----------------------------------------------------------------------------------------