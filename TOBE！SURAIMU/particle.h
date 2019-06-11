#pragma once

// 板場 温樹

#include "object_base.h"

struct Milliseconds;
template <typename T>
class Timer;

//-----------------------------------------------------------------------------
// パーティクル
//-----------------------------------------------------------------------------
// --説明--
// init関数引数リスト
//  TextureFileName : 読み込む画像ファイルのパス
//  Position : 初期座標
//  Velocity : 移動量
//  Clock : 経過時間をカウントしているタイマー
//  LifeTimeSec : 生存時間( 秒 )
class Particle :
	public ObjectBase
{
public:
	Particle();
	~Particle();
public:
	virtual bool init(
		const wchar_t* const TextureFileName,
		const Vector2& Position,
		const Vector2& Velocity,
		const Timer<Milliseconds>& Clock,
		const long long LifeTimeMs);
	virtual void destroy() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool isAlive() override { return is_alive_; }

protected:
	ID3D11ShaderResourceView* texture_;
	Vector2 position_;
	Vector2 velocity_;

	const Timer<Milliseconds>* clock_ = nullptr;
	long long lifetime_ms_;
	bool is_alive_;
};