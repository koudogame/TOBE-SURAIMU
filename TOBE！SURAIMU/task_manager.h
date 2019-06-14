#pragma once

class ObjectBase;

// 更新タスクID
enum class TaskUpdate
{
    kBackground,
	kStar,
	kPlayer,
    kSercher,
	kWall,
	kParticle,
    kBonusIcon,
    kRanking,
};
// 描画タスクID
enum class TaskDraw
{
    kBackground,
	kParticle,
	kObject,
};


//-----------------------------------------------------------------------------
// タスクマネージャー
//-----------------------------------------------------------------------------
// --説明--
// getInstance関数    : 実体へのインスタンスを返却
// registerTask関数   : タスクを登録する( 引数でタスクの種類を指定する )
// unregisterTask関数 : タスクの登録を解除する
// allUpdate関数      : 登録されている全ての「更新」タスクを実行する
// allDraw関数        : 登録されている全ての「描画」タスクを実行する
// allSetOver関数     : 登録されている登録されている全てのオブジェクトにスクロール処理をする
// pause関数          : 更新をrestart関数が呼ばれるまで一時停止する
// restart関数        : 更新を再開する
class TaskManager
{
private:
	//singleton
	TaskManager();

public:
	~TaskManager();


	static TaskManager* getInstance() { static TaskManager instance; return &instance; }


	void registerTask(ObjectBase* const, const TaskUpdate);
	void registerTask(ObjectBase* const, const TaskDraw);
	void unregisterObject(ObjectBase* const);


	void allUpdate();
	void allDraw();
	void allSetOver( const float Over );
    inline void pause()   { pause_ = true; }
    inline void restart() { pause_ = false; }


private:
	template <typename T>
	void insertTask(std::list<std::pair<T, ObjectBase*>>* const, ObjectBase* const, const T&);

	template <typename T>
	void removeTask(std::list<std::pair<T, ObjectBase*>>* const, ObjectBase* const);

	template <typename T>
	void execute(std::list<std::pair<T, ObjectBase*>>* const, void(ObjectBase::*)());


    bool pause_ = false;
	std::list<std::pair<TaskUpdate, ObjectBase*>> update_list_;
	std::list<std::pair<TaskDraw, ObjectBase*>> draw_list_;
};
