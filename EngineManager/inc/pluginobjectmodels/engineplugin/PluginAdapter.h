#ifndef ACTOR_ADAPTER_H
#define ACTOR_ADAPTER_H

#include <iostream>
#include <pluginobjectmodels/engineplugin/PluginModel.h>

class PluginAdapter : public IPluginModel
{
public:
	PluginAdapter(CPlugin * actor, PF_DestroyFunc destroyFunc) : pluG(actor),destroyFunc_(destroyFunc)
	{
		std::cout<<"PluginAdapter"<<std::endl;
	}

	virtual ~PluginAdapter()
	{
		if (destroyFunc_)
			destroyFunc_(pluG);
	}

	void getInitialInfo(PluginInfo * info)
	{
		pluG->getInitialInfo(pluG->handle, info);
	}
	void setUP(	RLMEngine::RLMlistenerFunctor	*RLMMakerLisener,
				RLMEngine::RLMlistenerFunctor	*RLMTakerLisener,
				RLMEngine::RLMEngineManager		*RLMengine)
	{
		pluG->setUP(pluG->handle,RLMMakerLisener,RLMTakerLisener,RLMengine);
	}
	void run()
	{
		pluG->run(pluG->handle);
	}

private:
	CPlugin				*pluG;
	PF_DestroyFunc		destroyFunc_;
};

#endif
