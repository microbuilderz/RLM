#include "pluginFramework/plugin.h"
#include "pluginFramework/PluginRegisterer.h"
#include "RLMEngineDynamic.h"

extern "C" PLUGIN_API int ExitFunc()
{
	return 0;
}

extern "C" PLUGIN_API PF_ExitFunc PF_initPlugin(const PF_PlatformServices * params)
{
	int res = 0;

	PF_RegisterParams rp;
	rp.type			 		= PF_ENGINE;
	rp.version.major 		= 1;
	rp.version.minor 		= 0;
	rp.programmingLanguage 	= PF_ProgrammingLanguage_CPP;

	rp.createFunc			= RLMEngineDynamic::create;
	rp.destroyFunc			= RLMEngineDynamic::destroy;
#ifdef DynamicEnginePlugin_PT_EXPORTS
	res = params->registerObject((const unsigned char *)"PF_Engine_PT", &rp);
#endif
#ifdef DynamicEnginePlugin_NT_EXPORTS
    res = params->registerObject((const unsigned char *)"PF_Engine_NT", &rp);
#endif
#ifdef DynamicEnginePlugin_NORCLIENT1_EXPORTS
	res = params->registerObject((const unsigned char *)"PF_Engine_NORCLIENT", &rp);
#endif
	if (res < 0)
		return NULL;

	return ExitFunc;
}
