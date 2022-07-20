#pragma once
#include <iostream>
#include <chrono>

//Credits to The Cherno: https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw

class ScopedTimer
{
public:
	ScopedTimer();
	~ScopedTimer();
	void stop();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
};