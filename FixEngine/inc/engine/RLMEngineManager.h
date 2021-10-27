#pragma once

#include "RLMconf.hpp"
#include "engine/RLMEngineListener.h"
#include "engine/makers/MakersListener.h"
#include "engine/takers/TakersListener.h"

namespace RLMEngine
{


	class RLMEngineManager
	{
	public:
						RLMEngineManager(RLMEngine::NetworkConnectionType connectionType,RLMEngine::RLMMode jobMode,RLMEngine::RLMlistenerFunctor *ptr1,RLMEngine::RLMlistenerFunctor *ptr2);
		virtual 		~RLMEngineManager();

		void			updateFunctors(RLMEngine::RLMlistenerFunctor *ptr1,RLMEngine::RLMlistenerFunctor *ptr2);

		// RLMEngine
		EngineSettings	getEngineSettings() { return engineSettings;}
		void			setListeningPort(RLMEngine::Port Port) {listeningPort = Port;}
		void			initEngine();
		void			registerEngineListener();
		void			shutDownEngine();

		// Takers
		void			initTakersSession(string senderCompId_,string targetCompId_);
		void			loginTakersSession();
		void		    logoutTakersSession();
        int             getTakersCount()    {return takerscount;}
		Session**		getTakersSession() {return takerSession;}

		// Makers
		void			initMakersSession(string senderCompId_,string targetCompId_);
		void			loginMakersSession(string counterPartyHost,int counterPartyPort, int heartBtInt, Message *customLogon = NULL);
		void			logoutMakersSession();
		Session*		getMakerSession() {return makerSession;}


	private:
		EngineSettings						engineSettings;
        int                                 takerscount;
        Session**                           takerSession;
		Session								*makerSession;

		RLMEngine::RLMEngineListener 		*engineListener;
		RLMEngine::Port						listeningPort;
		RLMEngine::NetworkConnectionType	connectionMode;
		RLMEngine::RLMMode					engineMode;

		RLMEngine::TakersListener			*takersListener;
		RLMEngine::MakersListener			*makersListener;

	};
}
