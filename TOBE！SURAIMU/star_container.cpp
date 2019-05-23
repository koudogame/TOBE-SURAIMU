
// 板場 温樹

#include "star_container.h"

#include "release.h"


/*===========================================================================*/
StarContainer::StarContainer()
{}

StarContainer::~StarContainer()
{
}

/*===========================================================================*/
// スターの追加
Star* StarContainer::addStar(
	const Vector2& Position, const float Angle,
	const float Spin, const float Rate, const float Size)
{
	// フリーなスターを取得
	Star* star = getFreeObjAndInsert();
	if (star == nullptr) { return nullptr; }

	// スターの初期化処理
	star->init(
		Position,
		Angle,
		Spin,
		Rate,
		Size
	);

	return star;
}


/*===========================================================================*/
// スターのステータスに倍率をかける
void StarContainer::resetStates(const float Magnification)
{
    for (auto& star : active_list_)
    {
        star->resetStatus(Magnification);
    }
}

/*===========================================================================*/
// y座標の移動( スクロール )
void StarContainer::setMove(const float Move)
{
    for (auto& star : active_list_)
    {
        star->setMove(Move);
    }
}
