#pragma once

struct Seconds;
struct Milliseconds;

//-----------------------------------------------------------------------------
// タイマー
//-----------------------------------------------------------------------------
// --説明--
// コンストラクタ又は、start関数でタイマースタート
// stop関数でスタートからの経過時間を取得
// ↑テンプレートパラメータで取得する時間の単位を選択する
class Timer
{
public:
	Timer();
	~Timer();
public:
	void start();
	void stop();
	void restart();

	template <typename T>
	long long getElapsedTime();

private:
	bool stop_ = false;
	std::chrono::high_resolution_clock::time_point start_;
	std::chrono::high_resolution_clock::time_point middle_;
};