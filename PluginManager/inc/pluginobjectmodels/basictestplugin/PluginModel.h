#ifndef OBJECT_MODEL
#define OBJECT_MODEL

#include "pluginobjectmodels/basictestplugin/CPluginModel.h"

typedef CPluginInfo	PluginInfo;

struct				IPluginModel
{
	virtual void	getInitialInfo(PluginInfo * info)	= 0;
	virtual void	run()								= 0;
};

#endif
