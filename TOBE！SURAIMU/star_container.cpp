
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
// 更新処理
void StarContainer::update()
{
	for (auto itr = active_list_.begin(), end = active_list_.end();
		itr != end;)
	{
		// 星の更新
		(*itr)->update();
		if ((*itr)->isLife() == false)		// 死んだらフリーリストに追加
		{
			(*itr)->destroy();
			free_list_.push_back((*itr));
			itr = active_list_.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

/*===========================================================================*/
// 終了処理
void StarContainer::destroy()
{
	// アクティブなスターの開放処理
	for (auto itr = active_list_.begin(), end = active_list_.end();
		itr != end;)
	{
		(*itr)->destroy();
		safe_delete(*itr);
		itr = active_list_.erase(itr);
	}
	
	// フリーリストなスターの開放処理
	Star* star = nullptr;
	while (free_list_.size())
	{
		star = free_list_.back();

		star->destroy();
		safe_delete(star);
		free_list_.pop_back();
	}
}

/*===========================================================================*/
// スターの追加
void StarContainer::addStar(
	const Vector2& Position, const float Angle, const float Fall,
	const float Spin, const float Rate, const float Size)
{
	Star* star = popFreeObj();
	if (star == nullptr)
	{
		star = new Star(task_manager_);
		active_list_.push_back(star);
	}

	star->init(
		Position,
		Angle,
		Fall,
		Spin,
		Rate,
		Size
	);
}
