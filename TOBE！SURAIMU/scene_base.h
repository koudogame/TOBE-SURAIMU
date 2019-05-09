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
// update関数では遷移先のシーンポインタを返却する( 遷移無しの場合はthis )
class SceneBase
{
public:
	SceneBase(const SceneID ID) : id_(ID) {}
	virtual ~SceneBase() = default;
public:
	virtual bool init() = 0;
	virtual void destroy() = 0;
	virtual SceneBase* update() = 0;
	virtual void draw() = 0;

protected:
	ID3D11ShaderResourceView* texture_ = nullptr;

private:
	SceneID id_;
};
