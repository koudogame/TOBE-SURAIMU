
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
	virtual void update() = 0;
	virtual void destroy() = 0;

protected:
	T* popFreeObj()
	{
		T* free_obj = nullptr;
		if (free_list_.size())
		{
			free_obj = free_list_.back();
			free_list_.pop_back();
		}
		return free_obj;
	}


protected:
	TaskManager* task_manager_ = nullptr;
	std::list<T*> active_list_;
	std::vector<T*> free_list_;
};