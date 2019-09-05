
// 板場　温樹

#include "timer.h"

using namespace std::chrono;
using Clock = high_resolution_clock;


/*===========================================================================*/
template <typename T>
Timer<T>::Timer()
{}

template <typename T>
Timer<T>::~Timer()
{}

/*===========================================================================*/
// 計測開始
template <typename T>
void Timer<T>::start()
{
	start_ = Clock::now();
	count_ = 0LL;
}

/*===========================================================================*/
// 計測一時停止
template <>
void Timer<Seconds>::stop()
{
	if (stop_ == false)
	{
		count_ += duration_cast<seconds>(getElapsedTime()).count();
		stop_ = true;
	}
}
template <>
void Timer<Milliseconds>::stop()
{
	if (stop_ == false)
	{
		count_ += duration_cast<milliseconds>(getElapsedTime()).count();
		stop_ = true;
	}
}

/*===========================================================================*/
// 計測再開始
template <typename T>
void Timer<T>::restart()
{
	stop_ = false;
	start_ = Clock::now();
}

/*===========================================================================*/
// 計測時間返却
template <>
long long Timer<Seconds>::getCount() const
{
	return count_ + duration_cast<seconds>(getElapsedTime()).count();
}
template <>
long long Timer<Milliseconds>::getCount() const
{
	return count_ + duration_cast<milliseconds>(getElapsedTime()).count();
}


/*===========================================================================*/
// 経過時間の取得
template <typename T>
std::common_type_t<Clock::duration, Clock::duration> Timer<T>::getElapsedTime() const
{
	return (stop_ ? start_ : Clock::now()) - start_;
}

template Timer<Seconds>;
template Timer<Milliseconds>;
