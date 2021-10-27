#pragma once

#include <vector>
#include <map>
#include "pluginobjectmodels/basictestplugin/PluginModel.h"

struct PF_ObjectParams;

class RLMEnginePluggable : public IPluginModel
{
public:
	static void 	*create(PF_ObjectParams *);
	static int		destroy(void *);
	virtual void	getInitialInfo(PluginInfo *info);
	virtual void	run();

	virtual			~RLMEnginePluggable();
  
private:
					RLMEnginePluggable();
};
