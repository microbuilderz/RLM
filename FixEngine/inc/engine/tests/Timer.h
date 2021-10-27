#pragma once

namespace RLMTest
{

class Timer
{
	public:
						Timer();
		virtual 		~Timer();

		void			resetTimer();
		void			startTimer();
		void			stopTimer();
        void            outputStartTime(string str);
        void            outputEndTime(string str);
		void			outputTime(string str);
		void			outputDiffTime(string str);
		void			outputTime(string str,timespec timePoint);
		timespec		getStartTime()	{return start;}
		timespec		getEndTime()	{return end;}
		timespec		diff();

	private:
		timespec start;
		timespec end;
	};
}
