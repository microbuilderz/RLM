#include "pluginFramework/plugin.h"
#include "pluginFramework/PluginRegisterer.h"
#include "RLMEnginePluggable.h"
#include <iostream>

extern "C" int				StaticPlugin_ExitFunc()
{
	return 0;
}

extern "C" PF_ExitFunc		StaticPlugin_InitPlugin(const PF_PlatformServices * params)
{
	PF_RegisterParams rp;
	int res						= 0;
	rp.version.major			= 1;
	rp.version.minor			= 0;
	rp.programmingLanguage		= PF_ProgrammingLanguage_CPP;

	rp.createFunc				= RLMEnginePluggable::create;
	rp.destroyFunc				= RLMEnginePluggable::destroy;
	res = params->registerObject((const unsigned char *)"RLMEnginePluggable", &rp);
	if (res < 0)
		return NULL;
	std::cout<<"end StaticPlugin_InitPlugin"<<std::endl;
	return StaticPlugin_ExitFunc;
}

//RLMFrameWork::PluginRegisterer StaticPlugin_registerer(RLMFrameWork::PluginRegisterer);
