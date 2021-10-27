#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "EngineManager.h"
#include "pluginobjectmodels/basictestplugin/PluginFactory.h"

namespace RLMEngineManager
{
	EngineManager & EngineManager::getInstance()
	{
		static EngineManager em;
		return em;
	}

	void EngineManager::scan()
	{
		RLMFrameWork::PluginManager & pm = RLMFrameWork::PluginManager::getInstance();
		const RLMFrameWork::PluginManager::RegistrationMap & rm = pm.getRegistrationMap();
        std::vector<std::string> 		PluginTypeVec;
		for (RLMFrameWork::PluginManager::RegistrationMap::const_iterator i = rm.begin(); i != rm.end(); ++i)
		{
			PluginTypeVec.push_back(i->first);
		}
		for (std::vector<std::string>::iterator i = PluginTypeVec.begin(); i != PluginTypeVec.end(); ++i)
		{
			std::string m = *i;
			std::cout << m.c_str() << std::endl;
		}

		const std::string & key = PluginTypeVec[0];

		IPluginModel * plugin1 = PluginFactory::createPlugin(key);

		plugin1->run();
	}

	EngineManager::EngineManager()
	{

	}

	EngineManager::~EngineManager()
	{
	}
}
