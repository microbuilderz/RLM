#ifdef WIN32
	#ifdef CPP_PLUGIN_EXPORTS
		#define PLUGIN_API __declspec(dllexport)
	#else
		#define PLUGIN_API __declspec(dllimport)
	#endif
#endif

