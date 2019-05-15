#pragma once

// 板場 温樹

class TaskManager;

// 各オブジェクトID
enum class ObjectID
{
	kPlayer,
	kStar,
	kWall,
	kItem,
	kParticle,
};

//-----------------------------------------------------------------------------
// オブジェクトの基底クラス
//-----------------------------------------------------------------------------
// --説明--
// コンストラクタでIDの初期化を行う
class ObjectBase
{
public:
	ObjectBase(const ObjectID ID, TaskManager* const TaskManager) :
		id_(ID),
		task_manager_(TaskManager)
	{}
	virtual ~ObjectBase() = default;
public:
	virtual bool init() { return true; }
	virtual void destroy() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool isAlive() { return true; }

	virtual ObjectID getId() { return id_; }

protected:
	ID3D11ShaderResourceView* texture_ = nullptr;
	TaskManager* task_manager_;
	Vector2 position_;


private:
	ObjectID id_;
};

