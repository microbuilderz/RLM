#include "engine/Linuxsignals.h"
#include "engine/RLMEngineListener.h"
#include "engine/RLMEngineManager.h"

namespace RLMEngine
{
	RLMEngineManager::RLMEngineManager(	RLMEngine::NetworkConnectionType connectionType,
										RLMEngine::RLMMode jobMode,
										RLMEngine::RLMlistenerFunctor *ptr1 /*Maker function */,
										RLMEngine::RLMlistenerFunctor *ptr2 /*Taker function*/)
	{
		manageLinuxSignals();
		makersListener					= new RLMEngine::MakersListener(ptr1);
		takersListener					= new RLMEngine::TakersListener(ptr2);

		engineListener					= NULL;
        takerscount                     = 0;
        takerSession                    = NULL;
		makerSession					= NULL;
		connectionMode					= connectionType;
		engineMode						= jobMode;
		switch (connectionMode)
		{
			case RLMEngine::SSL:
				listeningPort			= RLMEngine::DEFAULT_SSL_PORT;
			break;
			default:
				listeningPort			= RLMEngine::DEFAULT_PORT;
			break;
		}

		cout<<FGREEN<<"Engine Setup Completed"<<NONE<<endl;
	}

	void RLMEngineManager::updateFunctors(RLMEngine::RLMlistenerFunctor *ptr1,RLMEngine::RLMlistenerFunctor *ptr2)
	{
		makersListener->setupFunctor(ptr1);
		takersListener->setupFunctor(ptr2);
	}

	RLMEngineManager::~RLMEngineManager()
	{
		delete engineListener;
        for (int i=0;i<takerscount;i++)
		    free(takerSession[i]);
        free(takerSession);
        takerSession=NULL;
		delete makerSession;
		delete takersListener;
		delete makersListener;
	}

	void RLMEngineManager::initEngine()
	{
		switch (connectionMode)
		{
			case RLMEngine::SSL:
				clog << "SSL Mode." << endl;
				if (engineMode==RLMEngine::Acceptor)
				{
					engineSettings.sslListenPort (listeningPort);
					engineSettings.sslCertificateFile ("test.pem");
					engineSettings.sslPrivateKeyFile ("test.pem");
				}
				break;
			default:

				if (engineMode==RLMEngine::Acceptor)
					engineSettings.listenPort (listeningPort);
				break;
		}

		if (engineMode==RLMEngine::Acceptor)
		{
			engineSettings.sendLogoutOnInvalidLogon (true);
			engineSettings.sendLogoutOnException(true);
		}

		engineSettings.licenseStore("/home/meditation/eclipse/workspace/RLM_test/lic");
		Engine::init(engineSettings);
		cout<<FGREEN<<"Engine Started"<<NONE<<endl;
	}

	void RLMEngineManager::registerEngineListener()
	{
		engineListener	=new RLMEngineListener();
		Engine::instance()->registerListener(engineListener);
		cout<<FYELLOW<<"Client listener registered "<<NONE<<endl;
	}

	void RLMEngineManager::shutDownEngine()
	{
		cout<<FRED<<"Shutting down Engine"<<endl;
		if (engineMode==RLMEngine::Acceptor)
		{
			if (engineListener!=NULL)
				Engine::instance()->unregisterListener();
		}
		Engine::shutdown ();
		cout<<FRED<<"Engine Shutdown COMPLETED"<<NONE<<endl;
	}

	void RLMEngineManager::initTakersSession(	string senderCompId_,
												string targetCompId_)
	{
		const std::string senderCompId = senderCompId_;//"SellSide";
		const std::string targetCompId = targetCompId_; //"BuySide";

		Session* taker_Session = new Session(senderCompId, targetCompId, fixProtocolVersion, takersListener,SessionStorageType::AsyncFileBased);
        taker_Session->logInboundMessages(FIXLOGINBOUND);
        takerscount++;
        takerSession = (Session **) realloc(takerSession, takerscount * sizeof(Session*));

        takerSession[takerscount-1]=taker_Session;
        //takerSession.push_back(taker_Session);
#ifdef FIXENGINELOG
        takerSession->logDirectory((char*)FIXENGINELOG);
		cout<<" Taker Session Inbound logging state " <<FGREEN<<taker_Session->logInboundMessages()<<NONE<<endl;
#endif


		switch (connectionMode)
		{
			case RLMEngine::SSL:
                taker_Session->encryptionMethod (EncryptionMethod::SSL);
				break;
			default:
				break;
		}
		cout<<FBLUE<<"Taker session initiated successfully "<<NONE<<endl;
	}

	void RLMEngineManager::loginTakersSession()
	{
        //for (int i=0;i<takerSession.size();i++)
		    takerSession[takerscount-1]->logonAsAcceptor();
		cout <<FBLUE<<"Awaiting session-initiator on port " << listeningPort << "..." << NONE<< endl;
	}

	void RLMEngineManager::logoutTakersSession()
	{
		cout<<FRED<<"Taker session logout "<<NONE<<endl;
        for (int i=0;i<takerscount;i++)
        {
		    takerSession[i]->logout();
		    takerSession[i]->shutdown();
        }
	}

	void RLMEngineManager::initMakersSession(	string senderCompId_,
												string targetCompId_)
	{
		const string senderCompId = senderCompId_;
		const string targetCompId = targetCompId_;

		makerSession = new Session(senderCompId, targetCompId, fixProtocolVersion, makersListener,SessionStorageType::AsyncFileBased);
        makerSession->logInboundMessages(FIXLOGOUTBOUND);
#ifdef FIXENGINELOG
        makerSession->logDirectory((char*)FIXENGINELOG);
#endif
        cout<<" Maker Session Inbound logging state " <<FGREEN<<makerSession->logInboundMessages()<<NONE<<endl;

		switch (connectionMode)
		{
			case RLMEngine::SSL:
				makerSession->encryptionMethod (EncryptionMethod::SSL);
				break;
			default:
				break;
		}
	}

	void RLMEngineManager::loginMakersSession(	string counterPartyHost,
												int counterPartyPort,
												int heartBtInt,
												Message *customLogon)
	{
		if (customLogon!=NULL)
			makerSession->logonAsInitiator (counterPartyHost, counterPartyPort, heartBtInt, customLogon);
		else
			makerSession->logonAsInitiator (counterPartyHost, counterPartyPort, heartBtInt);
		cout<<FYELLOW<<"Maker session logged in "<<NONE<<endl;
	}

	void RLMEngineManager::logoutMakersSession()
	{
		cout<<FRED<<"Maker session logging out "<<NONE<<endl;
		makerSession->logout ();
		makerSession->shutdown ();
	}
}

