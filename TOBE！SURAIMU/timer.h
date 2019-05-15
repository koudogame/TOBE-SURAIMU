#pragma once

// 板場 温樹

struct Seconds;      // 計測時間単位 : 秒
struct Milliseconds; // 計測時間単位 : ミリ秒
	
//-----------------------------------------------------------------------------
// タイマー
//-----------------------------------------------------------------------------
// --説明--
// テンプレートパラメータで単位を指定する
// コンストラクタ、start関数で計測がスタート
// start関数   : 計測をスタートする( 経過時間をリセットしてストップを解除する )
// stop関数    : 計測をストップする
// restart関数 : 計測を再スタートする
// getCount関数: 計測時間を返却する
template <typename T>
class Timer
{
public:

public:
	Timer();
	~Timer();
public:
	void start();
	void stop();
	void restart();

	long long getCount();

private:
	std::common_type_t<std::chrono::high_resolution_clock::duration, 
		std::chrono::high_resolution_clock::duration>
		getElapsedTime();

	bool stop_ = false;
	std::chrono::high_resolution_clock::time_point start_;
	unsigned long long count_ = 0LL;

};