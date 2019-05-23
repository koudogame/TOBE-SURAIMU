#pragma once

class ObjectBase;

// 更新タスクID
enum class TaskUpdate
{
    kBackgroundUpdate,
	kStarUpdate,
	kPlayerUpdate,
	kWallUpdate,
	kParticleUpdate,
    kComboUpdate,
};
// 描画タスクID
enum class TaskDraw
{
    kDraw,
};


//-----------------------------------------------------------------------------
// タスクマネージャー
//-----------------------------------------------------------------------------
// --説明--
// タスクの登録はregisterTask関数を使用
// オブジェクトごとタスクを削除する場合unregisterObject関数を使用
// 各タスクIDは列挙体「TaskUpdate」か「TaskDraw」に定義されている
// タスクはIDにより昇順に実行される
class TaskManager
{
	//singleton
private:
	TaskManager();
public:
	static TaskManager* getInstance() { static TaskManager instance; return &instance; }

public:
	~TaskManager();

public:
	void registerTask(ObjectBase* const, const TaskUpdate);
	void registerTask(ObjectBase* const, const TaskDraw);

	void unregisterObject(ObjectBase* const);

	void allUpdate();

	void allDraw();

	void allSetOver( const float Over );


private:
	template <typename T>
	void insertTask(std::list<std::pair<T, ObjectBase*>>* const, ObjectBase* const, const T&);

	template <typename T>
	void removeTask(std::list<std::pair<T, ObjectBase*>>* const, ObjectBase* const);

	template <typename T>
	void execute(std::list<std::pair<T, ObjectBase*>>* const, void(ObjectBase::*)());

private:
	std::list<std::pair<TaskUpdate, ObjectBase*>> update_list_;
	std::list<std::pair<TaskDraw, ObjectBase*>> draw_list_;
};
