#include "pluginFramework/plugin.h"
#include "RLMEnginePluggable.h"

extern "C" int				StaticPlugin_ExitFunc();
extern "C" PF_ExitFunc		StaticPlugin_InitPlugin(const PF_PlatformServices * params);
