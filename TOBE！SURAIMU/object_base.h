#pragma once

// 板場 温樹

class TaskManager;


enum ObjectID
{
    kNone,
    kPlayer,
    kStar,
    kWall,
	kSerch,
    kFailWall,
};

//-----------------------------------------------------------------------------
// オブジェクトの基底クラス
//-----------------------------------------------------------------------------
class ObjectBase
{
public:
	ObjectBase() = default;
	virtual ~ObjectBase() = default;
// メンバ関数
	virtual bool init()                         { return true; }
	virtual void destroy()                      = 0;
	virtual void update()                       = 0;
    virtual void draw()                         {};
    virtual void setMove( const float )         {};
	virtual bool isAlive()                      { return true; }
    virtual const Vector2& getPosition() const  { return Vector2::Zero; }
    virtual ObjectID getID()             const  { return ObjectID::kNone; }
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
		kPlayerNonCombo,
		kPlayerNowCombo,
		kNonParticle,
	};
}

