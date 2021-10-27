#ifndef PLUGIN_FACTORY
#define PLUGIN_FACTORY

#include <pluginFramework/PluginManager.h>
#include <pluginFramework/ObjectAdapter.h>
#include <pluginobjectmodels/engineplugin/PluginModel.h>
#include <pluginobjectmodels/engineplugin/PluginAdapter.h>

struct PluginFactory : public RLMFrameWork::ObjectAdapter<PluginAdapter, CPlugin>
{
	static PluginFactory	&getInstance()
	{
		static PluginFactory	instance;
		return instance;
	}

	static IPluginModel		*createPlugin(const std::string & objectType)
	{
		void * plugin = RLMFrameWork::PluginManager::getInstance().createObject(objectType, getInstance());
		return (IPluginModel *)plugin;
	}
};

#endif
