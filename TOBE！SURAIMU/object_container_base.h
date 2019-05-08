
class ObjectBase;
class TaskManager;

//-----------------------------------------------------------------------------
// オブジェクトコンテナの基底
//-----------------------------------------------------------------------------
// --説明--
// オブジェクトをdeleteせずに使いまわすことでnewのコストを抑える
// 派生クラスは、各々オブジェクトの追加関数を実装する
class ObjectContainerBase
{
public:
	ObjectContainerBase(TaskManager* const TaskManager) :
		task_manager_(TaskManager)
	{}
	virtual ~ObjectContainerBase() = default;
public:
	virtual void destroy();
	virtual void update();

protected:
	ObjectBase* getFreeObj();


private:
	TaskManager* task_manager_ = nullptr;
	std::list<ObjectBase*> active_list_;
	std::vector<ObjectBase*> free_list_;
};