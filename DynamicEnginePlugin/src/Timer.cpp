#include "RLMconf.hpp"
#include "Timer.h"


namespace RLMTest
{
	Timer::Timer() {

	}

	Timer::~Timer() {
	}

	void Timer::resetTimer()
	{
		clock_gettime(CLOCK_REALTIME, &start);
		clock_gettime(CLOCK_REALTIME, &end);
	}

	void Timer::startTimer()
	{
		clock_gettime(CLOCK_REALTIME, &start);
	}

	void Timer::stopTimer()
	{
		clock_gettime(CLOCK_REALTIME, &end);
	}

	void Timer::outputTime(string str)
	{
        timespec time;
        clock_gettime(CLOCK_REALTIME, &time);
        __time_t time_in_nanosec = 1000000000 * time.tv_sec + time.tv_nsec;
        clog<<str<<FYELLOW<<time_in_nanosec<<NONE<<endl;
	}

    void Timer::outputStartTime(string str)
    {
        __time_t timeInNanos = 1000000000 * start.tv_sec + start.tv_nsec;
        clog<<str<<FYELLOW<<timeInNanos<<NONE<<endl;
    }

    void Timer::outputEndTime(string str)
    {
        __time_t timeInNanos = 1000000000 * end.tv_sec + end.tv_nsec;
        clog<<str<<FYELLOW<<timeInNanos<<NONE<<endl;
    }

	void Timer::outputDiffTime(string str)
	{
		timespec diff=this->diff();
		clog<<str<<FGREEN<<(long long)diff.tv_sec<<"seconds and "<<diff.tv_nsec<<"nano seconds"<<NONE<<endl;
	}
	void Timer::outputTime(string str,timespec timePoint)
	{
		clog<<str<<FGREEN<<(long long)timePoint.tv_sec<<":"<<NONE<<timePoint.tv_nsec<<endl;
	}

	timespec Timer::diff()
	{
		timespec temp;
		if ((end.tv_nsec-start.tv_nsec)<0) {
			temp.tv_sec = end.tv_sec-start.tv_sec-1;
			temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
		} else {
			temp.tv_sec = end.tv_sec-start.tv_sec;
			temp.tv_nsec = end.tv_nsec-start.tv_nsec;
		}
		return temp;
	}
}
