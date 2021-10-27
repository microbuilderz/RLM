#include "engine/RLMEngineListener.h"
#include "engine/RLMEngineManager.h"
#include "engine/tests/testpointerfunctions.hpp"
#include "engine/adapter/SimpleLPAdapter.h"
#include "engine/adapter/SimpleClientAdapter.h"

RLMEngine::RLMEngineManager *RLMengine;	///< Main Engine Manager pointer \ref RLMEngine::RLMEngineManager

char*   mode    = NULL;

int     RLM_PORT    = 4500;
char    RLMSideName[255];
int     Port_index    = -1;

RLMEngine::SimpleLPAdapter      LPsAdapter;
RLMEngine::SimpleClientAdapter  TakersAdapter;

void    checkArgs(int argc, char** argv)
{
    if (argc<1)
    {
        cout <<FRED<<"Not enough or invalid arguments, please try again"<<NONE<<endl;
        cout <<FYELLOW<< "Usage : " << endl << "-p : for port index \n -rl : for RLM port \n -m : RLM/LP/CLient" <<NONE<< endl;
        exit(0);
    }
    for (int i = 1; i < argc; i++)
    {
        if (i + 1 != argc)
        {
            if (argv[i] == "--help")
            {
                cout <<FYELLOW<< "Usage : " << endl << "-p : for port index \n -rl : for RLM port \n -m : RLM/LP/CLient" <<NONE<< endl;
            }
            if (strcmp(argv[i],"-p")==0)
            {
                Port_index = atoi(argv[i + 1]);
                i++;
            }
            else if (strcmp(argv[i],"-rl")==0)
            {
                RLM_PORT = atoi(argv[i + 1]);
                i++;
            }
            else if (strcmp(argv[i],"-m")==0)
            {
                mode = argv[i + 1];
                i++;
            }
            else
            {
                cout <<FRED<<"Not enough or invalid arguments, please try again"<<NONE<<endl;
                cout <<FYELLOW<< "Usage : " << endl << "-lp : for port index \n -rl : for RLM port \n -m : RLM/LP/CLient" <<NONE<< endl;
                exit(0);
            }

        }

    }
}

RLMEngine::RLMEngineManager* initEngine(RLMEngine::NetworkConnectionType connectionType,
										RLMEngine::RLMMode mode,
										RLMEngine::Port port,
										RLMEngine::RLMlistenerFunctor *ptr1,
										RLMEngine::RLMlistenerFunctor *ptr2)
{
	RLMEngine::RLMEngineManager* RLMengine = new RLMEngine::RLMEngineManager(connectionType,mode,ptr1,ptr2);
	RLMengine->setListeningPort(port);
	RLMengine->initEngine();
	return RLMengine;
}

void	initInitiator(string host,int port,RLMEngine::RLMEngineManager *RLMengine,string senderCompId,string targetCompId)
{
	try
	{
		RLMengine->initMakersSession(senderCompId,targetCompId);
		RLMengine->loginMakersSession(host,port,30);
	}
	catch(const Exception& ex)
	{
		cout <<FRED<< "Exception: " << ex.what() <<NONE<< endl;
	}
}

void    initEngineListener(RLMEngine::RLMEngineManager *RLMengine)
{
    RLMengine->registerEngineListener();
}

void	initAcceptor(int port,RLMEngine::RLMEngineManager *RLMengine,string senderCompId,string targetCompId)
{
	try
	{
		RLMengine->initTakersSession(senderCompId,targetCompId);
		RLMengine->loginTakersSession();
	}
	catch(const Exception& ex)
	{
		cout <<FRED<< "Exception: " << ex.what() <<NONE<< endl;
	}
}

int		main(int argc, char** argv)
{
	int ret=system("clear");
    if (ret<0)
        cout<<FRED<<"system lib not accessible"<<NONE<<std::endl;

    checkArgs(argc,argv);

    if (Port_index<0 || RLM_PORT<0 || mode==NULL)
    {
        cout << FRED<<"Not enough or invalid arguments, please try again"<<NONE<<endl;
        cout <<FYELLOW<< "Usage : " << endl << "-p : for port index \n -rl : for RLM port \n -m : RLM/LP/CLient" <<NONE<< endl;
        exit(0);
    }

    sprintf(RLMSideName,"RLM_%d",RLM_PORT);

	if (strcmp(mode,"LP")==0)
	{
		cout<<FBLUE<<"Acceptor mode"<<NONE<<endl;

		RLMEngine::RLMlistenerFunctor *LPTakerListener=new RLMEngine::RLMlistenerFunctor();
		LPTakerListener->onInit 					= RLMTest::_Nothing;
		LPTakerListener->onInboundApplicationMsg	= RLMTest::_Nothing;
		LPTakerListener->onInboundSessionMsg		= RLMTest::_Nothing;
		LPTakerListener->onStateChangePtr			= RLMTest::_LPGenerator;

		RLMEngine::RLMlistenerFunctor *LPMakerListener=new RLMEngine::RLMlistenerFunctor();
		LPMakerListener->onInit 					= RLMTest::_Nothing;
		LPMakerListener->onInboundApplicationMsg 	= RLMTest::_Nothing;
		LPMakerListener->onInboundSessionMsg		= RLMTest::_Nothing;
		LPMakerListener->onStateChangePtr			= RLMTest::_Nothing;

        std::vector<RLMEngine::SideInfo> lpPorts=LPsAdapter.getLPInfos();
		RLMengine =initEngine(RLMEngine::NO_SSL,RLMEngine::Acceptor,lpPorts[Port_index].SidePort,LPMakerListener,LPTakerListener);
        initEngineListener(RLMengine);

		initAcceptor(lpPorts[Port_index].SidePort,RLMengine,lpPorts[Port_index].SideName,RLMSideName);

		getchar();
		RLMengine->logoutTakersSession();
		RLMengine->shutDownEngine();
		delete (RLMengine);
	}
	else if (strcmp(mode,"CLIENT")==0)
	{
		cout<<FBLUE<<"Initiator mode"<<NONE<<endl;

		RLMEngine::RLMlistenerFunctor *ClientTakerListener=new RLMEngine::RLMlistenerFunctor();
		ClientTakerListener->onInit 					= RLMTest::_Nothing;
		ClientTakerListener->onInboundApplicationMsg 	= RLMTest::_Nothing;
		ClientTakerListener->onInboundSessionMsg		= RLMTest::_Nothing;
		ClientTakerListener->onStateChangePtr			= RLMTest::_Nothing;

		RLMEngine::RLMlistenerFunctor *ClientMakerListener=new RLMEngine::RLMlistenerFunctor();
		ClientMakerListener->onInit 					= RLMTest::_Nothing;
		ClientMakerListener->onInboundApplicationMsg 	= RLMTest::_RLMTestClientLog;
		ClientMakerListener->onInboundSessionMsg		= RLMTest::_Nothing;
		ClientMakerListener->onStateChangePtr			= RLMTest::_Nothing;

		RLMengine =initEngine(RLMEngine::NO_SSL,RLMEngine::Initiator,0,ClientMakerListener,ClientTakerListener);
        initEngineListener(RLMengine);

        std::vector<std::string> clientsInfo=TakersAdapter.getClientInfos();
		initInitiator((char*)ENGINEHOSTNAME,RLM_PORT,RLMengine,clientsInfo[Port_index],RLMSideName);

        getchar();
		RLMengine->logoutMakersSession();
		RLMengine->shutDownEngine();
		delete (RLMengine);
	}
	else if (strcmp(mode,"RLM")==0)
	{
		cout<<FGREEN<<"RLM mode"<<NONE<<endl;

		RLMEngine::RLMlistenerFunctor *RLMTakerListener=new RLMEngine::RLMlistenerFunctor();
		RLMTakerListener->onInit 					= RLMTest::_Nothing;
		RLMTakerListener->onInboundApplicationMsg 	= RLMTest::_Nothing;
		RLMTakerListener->onInboundSessionMsg		= RLMTest::_Nothing;
		RLMTakerListener->onStateChangePtr			= RLMTest::_Nothing;

		RLMEngine::RLMlistenerFunctor *RLMMakerListener=new RLMEngine::RLMlistenerFunctor();
		RLMMakerListener->onInit 					= RLMTest::_RLMTestLPInitIncoming;
		RLMMakerListener->onInboundApplicationMsg 	= RLMTest::_RLMTestLPPassThrough;
		RLMMakerListener->onInboundSessionMsg		= RLMTest::_Nothing;
		RLMMakerListener->onStateChangePtr			= RLMTest::_Nothing;

		RLMengine =initEngine(RLMEngine::NO_SSL,RLMEngine::Acceptor,RLM_PORT,RLMMakerListener,RLMTakerListener);
        initEngineListener(RLMengine);

        // initialize LP Ports and LP Side names
        std::vector<RLMEngine::SideInfo> lpPorts=LPsAdapter.getLPInfos();
        for (int i =0;i<lpPorts.size();i++)
        {
            initInitiator((char*)ENGINEHOSTNAME,lpPorts[i].SidePort,RLMengine,RLMSideName,lpPorts[i].SideName);
        }

        // initialize Client Side names
        std::vector<std::string> clientsInfo=TakersAdapter.getClientInfos();
        for (int i =0;i<clientsInfo.size();i++)
        {
            initAcceptor(RLM_PORT,RLMengine,RLMSideName,clientsInfo[i]);
        }

		getchar();
		RLMengine->logoutTakersSession();
		RLMengine->logoutMakersSession();
		RLMengine->shutDownEngine();
		delete (RLMengine);
	}
	return 0;
}
