
#include "object_container_base.h"

#include "release.h"
#include "object_base.h"


void deleteAllElement(std::list<ObjectBase*>& List);
void deleteAllElement(std::vector<ObjectBase*>& List);


/*===========================================================================*/
// 終了処理
void ObjectContainerBase::destroy()
{
	deleteAllElement(active_list_);
	deleteAllElement(free_list_);
}

/*===========================================================================*/
// 更新処理
void ObjectContainerBase::update()
{
	for (auto itr = active_list_.begin(), end = active_list_.end();
		itr != end; )
	{
		// 死んでいたら、フリーリストに追加
		if ((*itr)->isLife() == false)
		{
			(*itr)->destroy();
			free_list_.push_back(*itr);
			itr = active_list_.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}


/*===========================================================================*/
// 空のオブジェクトを返却
ObjectBase* ObjectContainerBase::getFreeObj()
{
	ObjectBase* new_obj = nullptr;
	if (free_list_.size())
	{
		new_obj = free_list_.back();
		free_list_.pop_back();
		active_list_.push_back(new_obj);
	}
	return new_obj;
}




void deleteAllElement(std::list<ObjectBase*>& List)
{
	for (auto itr = List.begin(), end = List.end();
		itr != end;)
	{
		safe_delete(*itr);
		itr = List.erase(itr);
	}
}
void deleteAllElement(std::vector<ObjectBase*>& List)
{
	for (auto itr = List.begin(), end = List.end();
		itr != end;)
	{
		safe_delete(*itr);
		itr = List.erase(itr);
	}
}