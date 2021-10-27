/*
 * EngineManager.h
 *
 *  Created on: Jan 15, 2017
 *      Author: meditation
 */

#ifndef ENGINEMANAGER_H_
#define ENGINEMANAGER_H_

#include <RLMconf.hpp>

#include <engine/RLMEngineListener.h>
#include <engine/RLMEngineManager.h>

#include <engine/RLMEngineManager.h>

#include <pluginFramework/PluginManager.h>

#include <pluginobjectmodels/engineplugin/PluginFactory.h>
#include <pluginobjectmodels/engineplugin/PluginModel.h>

namespace RLMEngine
{
	class PEngineManager
	{
	public:
		virtual 						~PEngineManager();

		void 							setInstance(RLMEngine::NetworkConnectionType connectionType,
										 	 	 	RLMEngine::RLMMode mode,
                                                    RLMEngine::Port rlm_Port);
		static PEngineManager			&getInstance();
		void							scan();
		void							setFinishLoadCallback(int (*fp)(void*));
		int 							LoadPluginByInfoName(char* plugininfoname);
		void							setUPLatest();
        void                            saveServerMessage(char* message);
		RLMEngine::RLMEngineManager		*initEngine(RLMEngine::RLMlistenerFunctor *ptr1,
													RLMEngine::RLMlistenerFunctor *ptr2);

		void							UpdateFunctors();

	private:
										PEngineManager();
		void							initInitiator(string host,RLMEngine::Port,string senderCompId,string targetCompId);
		void							initAcceptor(int port,string senderCompId,string targetCompId);

		//RLMEngine::RLMEngineManager		*RLMengine;


		int								(*finsihedScan_Callback)(void*)	= 0;

		RLMEngine::RLMlistenerFunctor	*RLMTakerLisener;
		RLMEngine::RLMlistenerFunctor	*RLMMakerLisener;

		std::vector<std::string> 		PluginTypeVec;
		std::vector<IPluginModel*> 		PluginModelVec;

		RLMEngine::NetworkConnectionType connectionType;
		RLMEngine::RLMMode				mode;
		RLMEngine::Port			        RLM_Port;

		char    						RLMSideName[255];
        char                            AdminMessage[1024];
	};
}

#endif
