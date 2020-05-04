#include "Timer.h"

namespace Utils
{
	Timer::Timer()
	{
		_deltaTime = 0;
	}

	Timer::~Timer()
	{
	}

	void Timer::start()
	{
		_initTime = std::chrono::high_resolution_clock::now();
	}

	void Timer::stop()
	{
		auto finalTime = std::chrono::high_resolution_clock::now();
		auto dur = finalTime - _initTime;
		_deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	}

	long long Timer::getDeltaTime()
	{
		return _deltaTime;
	}

	double Timer::getFPS()
	{
		return 1000.0 / double(_deltaTime);
	}
}