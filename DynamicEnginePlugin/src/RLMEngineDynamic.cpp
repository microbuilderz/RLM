#include <string.h>
#include <iostream>

#include "RLMEngineDynamic.h"
#include "engine/tests/testpointerfunctions.hpp"

using namespace std;

RLMEngine::RLMEngineManager *RLMengine;

void * RLMEngineDynamic::create(PF_ObjectParams *)
{
	return new RLMEngineDynamic();
}

int RLMEngineDynamic::destroy(void * p)
{
	if (!p)
		return -1;
	delete (RLMEngineDynamic *)p;
	return 0;
}

RLMEngineDynamic::RLMEngineDynamic()
{
}

RLMEngineDynamic::~RLMEngineDynamic()
{
}

void RLMEngineDynamic::getInitialInfo(PluginInfo *info)
{
#ifdef DynamicEnginePlugin_PT_EXPORTS
    ::strcpy((char *)info->name, "PT");
#endif
#ifdef DynamicEnginePlugin_NT_EXPORTS
    ::strcpy((char *)info->name, "NT");
#endif
#ifdef DynamicEnginePlugin_NORCLIENT1_EXPORTS
    ::strcpy((char *)info->name, "NORCLIENT");
#endif
	info->id	= 0;
	this->info = info;
}

void RLMEngineDynamic::setUP(RLMEngine::RLMlistenerFunctor	*RLMMakerListener,RLMEngine::RLMlistenerFunctor	*RLMTakerListener,RLMEngine::RLMEngineManager *engine)
{
	printf("calling dynamic library RLMTakerListener = %p RLMTakerListener= %p\n",RLMTakerListener,RLMMakerListener);
	RLMengine = engine;
	RLMTakerListener->onInit 					= RLMTest::_Nothing;
	RLMTakerListener->onInboundApplicationMsg 	= RLMTest::_Nothing;
	RLMTakerListener->onInboundSessionMsg		= RLMTest::_Nothing;
	RLMTakerListener->onStateChangePtr			= RLMTest::_Nothing;

#ifdef DynamicEnginePlugin_PT_EXPORTS
	RLMMakerListener->onInit 					= RLMTest::_RLMTestLPInitIncoming;
	RLMMakerListener->onInboundApplicationMsg 	= RLMTest::_RLMTestLPPassThrough;
#endif
#ifdef DynamicEnginePlugin_NT_EXPORTS
    RLMMakerListener->onInit 					= RLMTest::_Nothing;
	RLMMakerListener->onInboundApplicationMsg 	= RLMTest::_Nothing;
#endif
#ifdef DynamicEnginePlugin_NORCLIENT1_EXPORTS
	RLMMakerListener->onInit		            = RLMTest::_RLMTestLPInitIncoming;
	RLMMakerListener->onInboundApplicationMsg   = RLMTest::_RLMTestNorClientOne;
#endif
	cout<<"setting up plugin"<<std::endl;
}

void RLMEngineDynamic::run(void)
{
	cout<<"running "<<info->name<<endl;
}


