#include <string>
#include <iostream>
#include <apr-1.0/apr_general.h>

#include "pluginFramework/PluginManager.h"
#include "utils/Path.h"
#include "EngineManager.h"
#include "static_plugin.h"

using namespace std;

int					DummyInvokeService(const unsigned char * serviceName, void * serviceParams)
{
  return 0;
}

int					main(int argc, char * argv[])
{
	cout<<"init pluginManager"<<endl;
	if (argc<2)
	{
		cout<<"need path"<<endl;
		return 0;
	}
	::apr_initialize();
	RLMFrameWork::PluginManager &pm = RLMFrameWork::PluginManager::getInstance();

	pm.getPlatformServices().invokeService = DummyInvokeService;
	pm.loadAll(Path::makeAbsolute(argv[1]));
	RLMFrameWork::PluginManager::initializePlugin(StaticPlugin_InitPlugin);
	RLMEngineManager::EngineManager::getInstance().scan();
	::apr_terminate();
	cout<<"end pluginManager"<<endl;
	return 0;
}
