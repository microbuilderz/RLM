#pragma once

#include "RLMconf.hpp"
#include "engine/tests/Timer.h"
#include "engine/RLMEngineManager.h"
#include "engine/tests/MassOrderGenerator.h"
#include "engine/takers/TakersListener.h"

extern RLMEngine::RLMEngineManager *RLMengine;

RLMTest::Timer *timer=new RLMTest::Timer();

void *threadMessageGenerator(void *arg)
{
	RLMEngine::RLMListenerStruct *_struct = (RLMEngine::RLMListenerStruct *)arg;

	Session *sn= _struct->sn;
	if (sn!=NULL)
	{
		if (sn->state()==SessionState::Active)
		{
			int i = 0;

			int startId=1607093;

			RLMTest::MassOrderGenerator *gen=new RLMTest::MassOrderGenerator(sn);
			while(++i < 10000000 )
			{
                Message* MassQuote =gen->GenerateMessage(startId++);
                usleep(10);
                sn->send(MassQuote);
#ifdef TERMINALLOG
                timer->outputTime("message generated time @ ");
#endif
				//
			}
		}
	}
	return NULL;
}

namespace RLMTest
{

    RLMTest::Timer 			*timer				= new RLMTest::Timer();

	int _LPInit(RLMEngine::RLMListenerStruct *ptr)
	{
		return 0;
	}
	int _LPGenerator(RLMEngine::RLMListenerStruct *ptr)
	{
		if (ptr->sn==NULL)
			return 0;
		if (ptr->sn->state()==SessionState::Active)
		{
			clog<<FGREEN<<"_LPGenerator sending"<<NONE<<std::endl;
			pthread_t pth;
			pthread_create(&pth,NULL,threadMessageGenerator,ptr);
			pthread_detach(pth);
		}
		return 0;
	}

	int _Nothing (RLMEngine::RLMListenerStruct *ptr)
	{
#ifdef TERMINALLOG
        if (ptr->msg)
            cout <<FPURPLE<< "\nIncoming application-level message:\n"<<NONE << *(ptr->msg) << endl;
#endif
		return 0;
	}

	int _RLMTestLPInitIncoming(RLMEngine::RLMListenerStruct *ptr)
	{
#ifdef TERMINALLOG
        if (ptr->msg)
            cout <<FPURPLE<< "\nIncoming application-level message:\n"<<NONE << *(ptr->msg) << endl;
#endif
		timer->resetTimer();
		return 0;
	}

    int _RLMTestNorClientOne(RLMEngine::RLMListenerStruct *ptr)
    {
        timer->startTimer();
#ifdef TERMINALLOG
        if (ptr->msg) {
            cout << FPURPLE << "\nIncoming application-level message:\n" << NONE << *(ptr->msg) << endl;
            timer->outputStartTime("incoming message time @ ");
        }
#endif
        Message *msg=ptr->msg;

        if (msg->type() == "i")
        {
            Session ** taker_sessions = RLMengine->getTakersSession();

            for (int i = 1; i < RLMengine->getTakersCount(); i++)
            {
                Session * sn=taker_sessions[i];
                msg->set(Tags::SenderCompID, sn->targetCompId());
                msg->set(Tags::TargetCompID, sn->senderCompId());
                sn->send(msg);

            }
            timer->stopTimer();
#ifdef TERMINALLOG
            timer->outputEndTime("message sending time @ ");
            timer->outputDiffTime("RLM engine time diff in nanosec ");
#endif
        }

        return 0;
    }

	int _RLMTestLPPassThrough(RLMEngine::RLMListenerStruct *ptr)
	{

        timer->startTimer();
#ifdef TERMINALLOG
        timer->outputStartTime("incoming message time @ ");
        if (ptr->msg)
            cout <<FPURPLE<< "\nIncoming application-level message:\n"<<NONE << *(ptr->msg) << endl;
#endif
        Message *msg=ptr->msg;

		if (msg->type() == "i") {
            Session ** taker_sessions = RLMengine->getTakersSession();

            for (int i = 0; i < RLMengine->getTakersCount(); i++)
            {
                Session * sn=taker_sessions[i];
                msg->set(Tags::SenderCompID, sn->targetCompId());
                msg->set(Tags::TargetCompID, sn->senderCompId());
                sn->send(msg);

            }
            timer->stopTimer();
#ifdef TERMINALLOG
            timer->outputEndTime("message sending time @ ");
#endif
            timer->outputDiffTime("RLM engine time diff in nanosec ");
		}

		return 0;
	}

	int _RLMTestClientLog(RLMEngine::RLMListenerStruct *ptr)
	{
#ifdef TERMINALLOG
        if (ptr->msg)
            cout <<FPURPLE<< "\nIncoming application-level message:\n"<<NONE << *(ptr->msg) << endl;
#endif
		return 0;
	}
}



