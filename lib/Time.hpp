#ifndef TIME_HPP
#define TIME_HPP

#include "MessageHandler.hpp"
#include <chrono>

enum TimeUnit:int{nanoseconds,microseconds,miliseconds,seconds,minutes,hours};

class Time{
	public:
		Time(TimeUnit unit = nanoseconds, bool inform = false);
		~Time();
		void start();
		double end();
	private:
		bool inform;
		int stage;
		double initialTime;
		double startTime;
		TimeUnit unit;
		double now();
		std::string getTimeUnitName();
};

#endif /* TIME_HPP */