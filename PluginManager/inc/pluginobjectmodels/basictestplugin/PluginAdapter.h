#ifndef ACTOR_ADAPTER_H
#define ACTOR_ADAPTER_H

#include <iostream>
#include "pluginobjectmodels/basictestplugin/PluginModel.h"

class PluginAdapter : public IPluginModel
{
public:
	PluginAdapter(CPlugin * actor, PF_DestroyFunc destroyFunc) : actor_(actor),destroyFunc_(destroyFunc)
	{
		std::cout<<"PluginAdapter"<<std::endl;
	}

	virtual ~PluginAdapter()
	{
		if (destroyFunc_)
			destroyFunc_(actor_);
	}

	void getInitialInfo(PluginInfo * info)
	{
		actor_->getInitialInfo(actor_->handle, info);
	}

	void run()
	{
		actor_->run(actor_->handle);
	}

private:
	CPlugin				*actor_;
	PF_DestroyFunc		destroyFunc_;
};

#endif
