#pragma once

#include <vector>
#include <map>

#include "RLMconf.hpp"

#include "pluginobjectmodels/engineplugin/PluginModel.h"
#include "engine/RLMEngineManager.h"

struct PF_ObjectParams;

class RLMEngineDynamic : public IPluginModel
{
public:
	static void						*create(PF_ObjectParams *);
	static int						destroy(void *);
	virtual							~RLMEngineDynamic();

	virtual void					getInitialInfo(PluginInfo *info);
	virtual void					run(void);
	virtual void					setUP(	RLMEngine::RLMlistenerFunctor	*RLMMakerListener,
											RLMEngine::RLMlistenerFunctor	*RLMTakerListener,
											RLMEngine::RLMEngineManager		*engine);
  
private:
									RLMEngineDynamic();
	PluginInfo 						*info;
};
