#pragma once
#include <chrono>

namespace Utils
{
	class Timer
	{
	public:
		Timer();
		~Timer();
		void start();
		void stop();

		//milliseconds
		long long getDeltaTime();
		double getFPS();
	private:
		std::chrono::time_point<std::chrono::steady_clock> _initTime;
		long long _deltaTime;
	};
}