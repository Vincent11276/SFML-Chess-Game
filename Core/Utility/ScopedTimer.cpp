#include "ScopedTimer.hpp"

//Credits to The Cherno: https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw

//starts internal timer
ScopedTimer::ScopedTimer()
{
	startTimepoint = std::chrono::high_resolution_clock::now();
}

//use of RAII to benchmark time
ScopedTimer::~ScopedTimer()
{
	stop();
}

//prints time elapsed between scope to console
void ScopedTimer::stop()
{
	std::chrono::steady_clock::time_point endTimepoint{ std::chrono::high_resolution_clock::now() };
	long long start{ std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count() };
	long long end{ std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count() };

	long long us{ end - start };
	double ms{ us * 0.001 };
	double s{ ms * 0.001 };

	std::cout << us << "us\n" << ms << "ms\n" << s << "s\n";
}