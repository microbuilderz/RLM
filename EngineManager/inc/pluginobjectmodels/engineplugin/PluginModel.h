#pragma once

#include <pluginobjectmodels/engineplugin/CPluginModel.h>


typedef CPluginInfo	PluginInfo;

struct				IPluginModel
{
	virtual void	getInitialInfo(PluginInfo * info)	= 0;
	virtual void	setUP(	RLMEngine::RLMlistenerFunctor	*RLMMakerLisener,
							RLMEngine::RLMlistenerFunctor	*RLMTakerLisener,
							RLMEngine::RLMEngineManager		*RLMengine)			= 0;
	virtual void	run()								= 0;
};

