#pragma once

// 各オブジェクトID
enum class ObjectID
{
	kPlayer,
	kStar,
	kWall,
	kItem,
};

//-----------------------------------------------------------------------------
// オブジェクトの基底クラス
//-----------------------------------------------------------------------------
// --説明--
// コンストラクタでIDの初期化を行う
class ObjectBase
{
public:
	ObjectBase(const ObjectID ID) : id_(ID) {}
	virtual ~ObjectBase() = default;
public:
	virtual bool init() { return true; }
	virtual void destroy() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool isLife() = 0;

	virtual ObjectID getId() { return id_; }

protected:
	ID3D11ShaderResourceView* texture_ = nullptr;
	Vector2 position_;


private:
	ObjectID id_;
};

