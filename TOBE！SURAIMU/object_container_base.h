
class ObjectBase;

//-----------------------------------------------------------------------------
// オブジェクトコンテナの基底
//-----------------------------------------------------------------------------
// --説明--
// オブジェクトをdeleteせずに使いまわすことでnewのコストを抑える
// 派生クラスは、各々オブジェクトの追加関数を実装する
class ObjectContainerBase
{
public:
	ObjectContainerBase() = default;
	virtual ~ObjectContainerBase() = default;
public:
	virtual void destroy();
	virtual void update();

protected:
	ObjectBase* getFreeObj();


private:
	std::list<ObjectBase*> active_list_;
	std::vector<ObjectBase*> free_list_;
};