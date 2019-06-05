#pragma once

// 板場 温樹

class TaskManager;


enum ObjectID
{
    kNone,
    kPlayer,
    kStar,
    kWall,
};

//-----------------------------------------------------------------------------
// オブジェクトの基底クラス
//-----------------------------------------------------------------------------
// --説明--
// コンストラクタでIDの初期化を行う
class ObjectBase
{
public:
	ObjectBase() = default;
	virtual ~ObjectBase() = default;
// メンバ関数
	virtual bool init() { return true; }
	virtual void destroy() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual bool isAlive() { return true; }
    virtual Vector2 getPosition() const { return position_; }
    virtual void setMove(const float Over) { position_.y += Over; }

    virtual ObjectID getID() { return ObjectID::kNone; }

protected:
	ID3D11ShaderResourceView* texture_ = nullptr;
	Vector2 position_;
};

//反則　TGSまでには修正します今は許して//***山田　晴久***
namespace NameSpaceParticle
{
	enum ParticleID
	{
		kCyan ,
		kYellow ,
		kMagenta ,
		kWall,
		kPlayer,
		kNonParticle,
	};
}

