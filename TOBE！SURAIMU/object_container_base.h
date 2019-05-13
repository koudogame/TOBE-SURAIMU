
#include "release.h"

class TaskManager;

//-----------------------------------------------------------------------------
// オブジェクトコンテナの基底
//-----------------------------------------------------------------------------
// --説明--
// 派生クラスは各々、要素の追加関数を実装する
// destroy関数は、呼び出されたら全要素の開放処理を行う
template <class T>
class ObjectContainerBase
{
public:
	ObjectContainerBase(TaskManager* const TaskManager) :
		task_manager_(TaskManager)
	{}
	virtual ~ObjectContainerBase() = default;

public:
	virtual void update()
	{
		for (auto itr = active_list_.begin(), end = active_list_.end();
			itr != end;)
		{
			// 死んだらフリーリストに追加
			if ((*itr)->isLife() == false)
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

	virtual void destroy()
	{
		// アクティブリストの開放処理
		for (auto itr = active_list_.begin(), end = active_list_.end();
			itr != end;)
		{
			(*itr)->destroy();
			safe_delete(*itr);
			itr = active_list_.erase(itr);
		}

		// フリーリストの開放処理
		T* obj = nullptr;
		while (free_list_.size())
		{
			obj = free_list_.back();

			safe_delete(obj);
			free_list_.pop_back();
		}
	}

	virtual const std::list<T*>& active() { return active_list_; }

protected:
	T* getFreeObjAndInsert()
	{
		T* free_obj = nullptr;
		if (free_list_.size())
		{
			free_obj = free_list_.back();
			free_list_.pop_back();
		}
		else
		{
			free_obj = new (std::nothrow) T(task_manager_);
		}
		active_list_.push_back(free_obj);
		return free_obj;
	}

protected:
	TaskManager* task_manager_ = nullptr;
	std::list<T*> active_list_;
	std::vector<T*> free_list_;
};