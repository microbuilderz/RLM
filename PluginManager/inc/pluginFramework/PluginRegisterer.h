
#ifndef PLUGIN_REGISTERER_H
#define PLUGIN_REGISTERER_H

#include "pluginFramework/PluginManager.h"

namespace RLMFrameWork
{
	struct PluginRegisterer
	{
		PluginRegisterer(PF_InitFunc initFunc)
		{
			PluginManager::initializePlugin(initFunc);
		}
	};
}

#endif
