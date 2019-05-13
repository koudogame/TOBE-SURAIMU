#pragma once

#include "object_base.h"

class TaskManager;

//-----------------------------------------------------------------------------
// パーティクル
//-----------------------------------------------------------------------------
class Particle :
	public ObjectBase
{
public:
	Particle(TaskManager* const);
	~Particle();
public:
	virtual bool init(
		const wchar_t* const TextureFileName, 
		const Vector2& Velocity,
		const long long LifeTimeMs);
	virtual void destroy() override;
	virtual void update() override;
	virtual void draw() override;
	virtual bool isLife() override { return is_alive_; }

private:
	ID3D11ShaderResourceView* texture_;
	Vector2 velocity_;

	std::chrono::high_resolution_clock::time_point prev_time_;
	long long lifetime_ms_;
	bool is_alive_;
};