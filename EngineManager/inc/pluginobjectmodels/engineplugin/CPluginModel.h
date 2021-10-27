#ifndef C_OBJECT_MODEL
#define C_OBJECT_MODEL

#define MAX_STR 64

#include <engine/RLMEngineManager.h>

typedef struct CPluginInfo_
{
	int					id;
	unsigned char		name[MAX_STR];
} CPluginInfo;

typedef struct CPluginHandle_ { char c; } * CPluginHandle;

typedef struct CPlugin_
{
	void 				(*getInitialInfo)(CPluginHandle handle, CPluginInfo * info);
	void				(*setUP)(	CPluginHandle 					handle,
									RLMEngine::RLMlistenerFunctor	*RLMMakerLisener,
									RLMEngine::RLMlistenerFunctor	*RLMTakerLisener,
									RLMEngine::RLMEngineManager		*RLMengine);
	void 				(*run)(CPluginHandle handle);
	CPluginHandle 		handle;
} CPlugin;

#endif
