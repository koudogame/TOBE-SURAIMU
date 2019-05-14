#pragma once

class Timer
{
public:
	Timer();
	~Timer();
public:
	void start();

	long long stop();
	long long lap();
	

private:
	std::chrono::high_resolution_clock::time_point start_;
};