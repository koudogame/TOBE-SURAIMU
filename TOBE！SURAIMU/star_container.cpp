
#include "star_container.h"

#include "release.h"
#include "star.h"


/*===========================================================================*/
StarContainer::StarContainer(TaskManager* const TaskManager) :
	ObjectContainerBase(TaskManager)
{}

StarContainer::~StarContainer()
{
}

/*===========================================================================*/
// スターの追加
Star* StarContainer::addStar(
	const Vector2& Position, const float Angle, const float Fall,
	const float Spin, const float Rate, const float Size)
{
	// フリーなスターを取得
	Star* star = getFreeObjAndInsert();
	if (star == nullptr) { return nullptr; }

	// スターの初期化処理
	star->init(
		Position,
		Angle,
		Fall,
		Spin,
		Rate,
		Size
	);

	return star;
}

/*===========================================================================*/
// スターに落下を追加
void StarContainer::setFall()
{
	for (auto& star : active_list_)
	{
		star->setFall();
	}
}
