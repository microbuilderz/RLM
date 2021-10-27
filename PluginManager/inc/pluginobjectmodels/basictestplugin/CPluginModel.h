#ifndef C_OBJECT_MODEL
#define C_OBJECT_MODEL

#define MAX_STR 64

typedef struct CPluginInfo_
{
	int					id;
	unsigned char		name[MAX_STR];
} CPluginInfo;

typedef struct CPluginHandle_ { char c; } * CPluginHandle;

typedef struct CPlugin_
{
	void 				(*getInitialInfo)(CPluginHandle handle, CPluginInfo * info);
	void 				(*run)(CPluginHandle handle);
	CPluginHandle 		handle;
} CPlugin;

#endif
