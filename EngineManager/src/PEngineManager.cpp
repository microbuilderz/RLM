#include "PEngineManager.h"
#include "engine/tests/testpointerfunctions.hpp"
#include "engine/adapter/SimpleLPAdapter.h"
#include "engine/adapter/SimpleClientAdapter.h"

extern RLMEngine::RLMEngineManager *RLMengine;

RLMEngine::SimpleLPAdapter      LPsAdapter;
RLMEngine::SimpleClientAdapter  TakersAdapter;

namespace RLMEngine
{
	PEngineManager &PEngineManager::getInstance()
	{
		static PEngineManager 			em;
		return em;
	}

	void PEngineManager::setInstance(	RLMEngine::NetworkConnectionType connectionType,
										RLMEngine::RLMMode mode,
										RLMEngine::Port rlm_Port)
	{
		this->connectionType		= connectionType;
		this->mode					= mode;
		this->RLM_Port				= rlm_Port;
        sprintf(AdminMessage,"%s","NONE");
        //this->LP_Port               = lp_Port;

		RLMTakerLisener->onInit 					= RLMTest::_Nothing;
		RLMTakerLisener->onInboundApplicationMsg 	= RLMTest::_Nothing;
		RLMTakerLisener->onInboundSessionMsg		= RLMTest::_Nothing;
		RLMTakerLisener->onStateChangePtr			= RLMTest::_Nothing;

		RLMMakerLisener->onInit 					= RLMTest::_RLMTestLPInitIncoming;
		RLMMakerLisener->onInboundApplicationMsg 	= RLMTest::_RLMTestLPPassThrough;
		RLMMakerLisener->onInboundSessionMsg		= RLMTest::_Nothing;
		RLMMakerLisener->onStateChangePtr			= RLMTest::_Nothing;


        sprintf(RLMSideName,"RLM_%d",rlm_Port);
        /*sprintf(LPSideName,"LPSide%d",LP_Port);
        sprintf(RLMSideName,"RLMSide%d",LP_Port);
        sprintf(ClientSideName,"ClientSide%d",RLM_Port);*/

		initEngine(RLMMakerLisener,RLMTakerLisener);
	}

	void  PEngineManager::UpdateFunctors()
	{
		RLMTakerLisener->onInit 					= RLMTest::_Nothing;
		RLMTakerLisener->onInboundApplicationMsg 	= RLMTest::_Nothing;
		RLMTakerLisener->onInboundSessionMsg		= RLMTest::_Nothing;
		RLMTakerLisener->onStateChangePtr			= RLMTest::_Nothing;

		RLMMakerLisener->onInit 					= RLMTest::_Nothing;
		RLMMakerLisener->onInboundApplicationMsg 	= RLMTest::_Nothing;
		RLMMakerLisener->onInboundSessionMsg		= RLMTest::_Nothing;
		RLMMakerLisener->onStateChangePtr			= RLMTest::_Nothing;
	}

	void PEngineManager::scan()
	{
		RLMFrameWork::PluginManager & pm = RLMFrameWork::PluginManager::getInstance();
		const RLMFrameWork::PluginManager::RegistrationMap & rm = pm.getRegistrationMap();
		for (RLMFrameWork::PluginManager::RegistrationMap::const_iterator i = rm.begin(); i != rm.end(); ++i)
		{
			if (i->second.type==PF_ENGINE)
				PluginTypeVec.push_back(i->first);
		}

		for (std::vector<std::string>::iterator i = PluginTypeVec.begin(); i != PluginTypeVec.end(); ++i)
		{
			std::string m = *i;
			const std::string & key = (*i);
			PluginModelVec.push_back(PluginFactory::createPlugin(key));
		}

		finsihedScan_Callback(AdminMessage);
	}

	PEngineManager::PEngineManager()
	{
		RLMengine		= NULL;
		RLMTakerLisener = new RLMEngine::RLMlistenerFunctor();
		RLMMakerLisener = new RLMEngine::RLMlistenerFunctor();

		printf("RLMTakerListener = %p RLMMakerListener= %p\n",RLMTakerLisener,RLMMakerLisener);

		connectionType	= RLMEngine::NO_SSL;
		mode			= RLMMode::Acceptor;
	}

	PEngineManager::~PEngineManager()
	{
		RLMengine->logoutTakersSession();
		RLMengine->logoutMakersSession();
		delete (RLMTakerLisener);
		delete (RLMMakerLisener);
		RLMengine->shutDownEngine();
		delete (RLMengine);
	}

    int PEngineManager::LoadPluginByInfoName(char* plugininfoname)
    {
        PluginInfo pf;
        for (int i=0;i<PluginModelVec.size();i++)
        {
            PluginModelVec.at(i)->getInitialInfo(&pf);
            if (strcmp(plugininfoname,(char*)pf.name)==0)
            {
                PluginModelVec.at(i)->setUP(RLMMakerLisener,RLMTakerLisener,RLMengine);
                return i;
            }
        }
        return -1;
    }

	void PEngineManager::setUPLatest()
	{
		if (PluginModelVec.size()>0)
			PluginModelVec.at(PluginModelVec.size()-1)->setUP(RLMMakerLisener,RLMTakerLisener,RLMengine);
	}

	void PEngineManager::setFinishLoadCallback(int (*fp)(void*))
	{
		finsihedScan_Callback = fp;
	}

	RLMEngine::RLMEngineManager* PEngineManager::initEngine(RLMEngine::RLMlistenerFunctor *ptr1,
															RLMEngine::RLMlistenerFunctor *ptr2)
	{
		RLMengine = new RLMEngine::RLMEngineManager(connectionType,mode,ptr1,ptr2);
		RLMengine->setListeningPort(RLM_Port);
		RLMengine->initEngine();
        RLMengine->registerEngineListener();
		/*initInitiator((char*)ENGINEHOSTNAME,LP_Port,RLMSideName,LPSideName);
		initAcceptor(RLM_Port,RLMSideName,ClientSideName);*/

        std::vector<RLMEngine::SideInfo> lpPorts=LPsAdapter.getLPInfos();
        for (int i =0;i<lpPorts.size();i++)
        {
            initInitiator((char*)ENGINEHOSTNAME,lpPorts[i].SidePort,RLMSideName,lpPorts[i].SideName);
        }

        // initialize Client Side names
        std::vector<std::string> clientsInfo=TakersAdapter.getClientInfos();
        for (int i =0;i<clientsInfo.size();i++)
        {
            initAcceptor(RLM_Port,RLMSideName,clientsInfo[i]);
        }

		return RLMengine;
	}

	void	PEngineManager::initInitiator(string host,RLMEngine::Port port,string senderCompId,string targetCompId)
	{
		try
		{
			RLMengine->initMakersSession(senderCompId,targetCompId);
			RLMengine->loginMakersSession(host,port,30);
		}
		catch(const Exception& ex)
		{
			cout << FRED << "Exception: " << ex.what() << NONE << endl;
		}
	}

	void	PEngineManager::initAcceptor(int port,string senderCompId,string targetCompId)
	{
		try
		{
			RLMengine->initTakersSession(senderCompId,targetCompId);
			RLMengine->loginTakersSession();
		}
		catch(const Exception& ex)
		{
			cout << FRED <<"Exception: " << ex.what() << NONE << endl;
		}
	}
    // TODO check if this will be accedded by threads, not implemented right now
    void PEngineManager::saveServerMessage(char *message)
    {
        sprintf(AdminMessage,"%s",message);
    }
}
