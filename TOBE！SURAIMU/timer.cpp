
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
long long Timer::stop()
{
	return duration_cast<milliseconds>(
		Clock::now() - start_
		).count();
}
