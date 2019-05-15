
// 板場 温樹

#include "task_manager.h"

#include "object_base.h"


/*===========================================================================*/
TaskManager::TaskManager()
{
}

TaskManager::~TaskManager()
{
}


/*===========================================================================*/
// 更新タスクの登録
void TaskManager::registerTask(ObjectBase* const Object, const TaskUpdate TaskID)
{
	insertTask(&update_list_, Object, TaskID);
}
// 描画タスクの登録
void TaskManager::registerTask(ObjectBase* const Object, const TaskDraw TaskID)
{
	insertTask(&draw_list_, Object, TaskID);
}

/*===========================================================================*/
// オブジェクトごとタスクを削除
void TaskManager::unregisterObject(ObjectBase* const Object)
{
	removeTask(&update_list_, Object);
	removeTask(&draw_list_, Object);
}

/*===========================================================================*/
// 更新タスクの実行
void TaskManager::allUpdate()
{
	execute(&update_list_, &ObjectBase::update);
}
// 描画タスクの実行
void TaskManager::allDraw()
{
	execute(&draw_list_, &ObjectBase::draw);
}

/*===========================================================================*/
// 引数のリストに、引数の要素を挿入
template <typename T>
void TaskManager::insertTask(
	std::list<std::pair<T, ObjectBase*>>* List,
	ObjectBase* const Object, const T& TaskID)
{
	// リストに"昇順"になるよう挿入
	for (auto itr = List->begin(), end = List->end();
		itr != end; ++itr) {
		if (itr->first >= TaskID)
		{
			List->insert(
				itr,
				std::make_pair(TaskID, Object)
			);
			return;
		}
	}
	List->push_back(std::make_pair(TaskID, Object));
}

/*===========================================================================*/
// 引数のリストから、引数の要素を削除
template <typename T>
void TaskManager::removeTask(
	std::list<std::pair<T, ObjectBase*>>* List,
	ObjectBase* const Object)
{
	for (auto itr = List->begin(), end = List->end();
		itr != end; ++itr) {
		if (itr->second == Object)
		{
			List->erase(itr);
			return;
		}
	}
}

/*===========================================================================*/
// 引数の関数ポインタをリストの全オブジェクトに対して実行
template <typename T>
void TaskManager::execute(
	std::list<std::pair<T, ObjectBase*>>* List,
	void(ObjectBase::*Func)())
{
	for (auto itr = List->begin(), end = List->end();
		itr != end; ++itr) {
		((itr->second)->*Func)();
	}
}
