
#include "timer.h"

using namespace std::chrono;
using Clock = high_resolution_clock;

/*===========================================================================*/
Timer::Timer()
{
	start_ = Clock::now();
}

Timer::~Timer()
{
}

/*===========================================================================*/
// 計測スタート
void Timer::start()
{
	start_ = Clock::now();
}

/*===========================================================================*/
// 計測ストップ
void Timer::stop()
{
	stop_ = true;
	middle_ = Clock::now();
}

/*===========================================================================*/
// 計測再スタート
void Timer::restart()
{
	stop_ = false;
}

/*===========================================================================*/
// 経過時間の取得( 秒 )
template <>
long long Timer::getElapsedTime<Seconds>()
{
	long long elapsed;
	if (stop_)
	{
		elapsed = duration_cast<seconds>(
			middle_ - start_
			).count();
	}
	else
	{
		elapsed = duration_cast<seconds>(
			Clock::now() - start_
			).count();
	}

	return elapsed;
}
// 経過時間の取得( ミリ秒 )
template <>
long long Timer::getElapsedTime<Milliseconds>()
{
	long long elapsed;
	if (stop_)
	{
		elapsed = duration_cast<milliseconds>(
			middle_ - start_
			).count();
	}
	else
	{
		elapsed = duration_cast<milliseconds>(
			Clock::now() - start_
			).count();
	}

	return elapsed;
}
