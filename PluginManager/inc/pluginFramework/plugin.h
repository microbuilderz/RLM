/*
 * plugin.h
 *
 *  Created on: Jan 15, 2017
 *      Author: meditation
 */

#ifndef PLUGIN_H_
#define PLUGIN_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum					_PF_ProgrammingLanguage
{
	PF_ProgrammingLanguage_C,
	PF_ProgrammingLanguage_CPP
} PF_ProgrammingLanguage;

struct PF_PlatformServices;

typedef struct					PF_ObjectParams
{
  const unsigned char 				*objectType;
  const struct PF_PlatformServices	*platformServices;
} PF_ObjectParams;

typedef enum					_PF_PluginType
{
	PF_TEST,
	PF_ENGINE,
	PF_LOGGER,
	PF_NONE
} PF_PluginType;

typedef  enum					_PF_PluginInterChange
{
	PF_NOTHING,
	PF_EXCHANGE,
	PF_MANUAL_EXCHANGE,
} PF_PluginInterChange;

typedef struct					PF_PluginAPI_Version
{
  int major;
  int minor;
} PF_PluginAPI_Version;

typedef void*		(*PF_CreateFunc)(PF_ObjectParams *);
typedef int 		(*PF_DestroyFunc)(void *);

typedef struct PF_RegisterParams
{
  PF_PluginType				type;
  PF_PluginAPI_Version		version;
  PF_CreateFunc				createFunc;
  PF_DestroyFunc			destroyFunc;
  PF_ProgrammingLanguage	programmingLanguage;
} PF_RegisterParams;

typedef int 		(*PF_RegisterFunc)		(const unsigned char * nodeType, const PF_RegisterParams * params);
typedef int 		(*PF_InvokeServiceFunc)	(const unsigned char * serviceName, void * serviceParams);

typedef struct PF_PlatformServices
{
  PF_PluginAPI_Version		version;
  PF_RegisterFunc			registerObject;
  PF_InvokeServiceFunc		invokeService;
} PF_PlatformServices;

typedef int 		(*PF_ExitFunc)();
typedef PF_ExitFunc (*PF_InitFunc)(const PF_PlatformServices *);

#define PLUGIN_API

extern
#ifdef  __cplusplus
"C"
#endif
PLUGIN_API PF_ExitFunc PF_initPlugin(const PF_PlatformServices * params);

#ifdef  __cplusplus
}
#endif

#endif
