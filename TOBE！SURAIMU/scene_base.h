#pragma once

// 各シーンID
enum class SceneID
{
	kTitle,
	kPlay,
	kResult,
};

//-----------------------------------------------------------------------------
// シーンの基底クラス
//-----------------------------------------------------------------------------
// --説明--
// コンストラクタでIDの初期化を行う
// update関数では遷移先のシーンIDを返却する( 遷移しない場合は、自身のID )
class SceneBase
{
public:
	SceneBase(const SceneID ID) : id_(ID) {}
	virtual ~SceneBase() = default;
public:
	virtual bool init() = 0;
	virtual void destroy() = 0;
	virtual SceneID update() = 0;
	virtual void draw() = 0;

private:
	SceneID id_;
};

