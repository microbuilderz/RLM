/*
 * PluginManager.h
 *
 *  Created on: Jan 15, 2017
 *      Author: meditation
 */

#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include <vector>
#include <map>

#include "pluginFramework/plugin.h"
#include "pluginFramework/ObjectAdapter.h"
#include "pluginFramework/DynamicLibrary.h"

namespace RLMFrameWork
{
	class PluginManager
	{
		typedef std::map<std::string, DynamicLibrary*>		DynamicLibraryMap;
		typedef std::vector<PF_ExitFunc>					ExitFuncVec;
		typedef std::vector<PF_RegisterParams>				RegistrationVec;

	public:
		typedef std::map<std::string, PF_RegisterParams> 	RegistrationMap;

		static PluginManager 			&getInstance();
		static int						initializePlugin(PF_InitFunc initFunc);
		int								loadAll(const std::string &pluginDirectory, PF_InvokeServiceFunc func = NULL);
		int								loadByPath(const std::string &path);

		void 							*createObject(const std::string &objectType, IObjectAdapter &adapter);

		int								shutdown();
		static int						registerObject(const unsigned char *nodeType, const PF_RegisterParams *params);
		const RegistrationMap 			&getRegistrationMap();
		PF_PlatformServices 			&getPlatformServices();

	private:
										PluginManager();
										PluginManager(const PluginManager &);
		virtual							~PluginManager();
		RLMFrameWork::DynamicLibrary*	loadLibrary(const std::string &path, std::string &errorString);

		bool              				inInitializePlugin_;
		PF_PlatformServices				platformServices_;
		DynamicLibraryMap				dynamicLibraryMap_;
		ExitFuncVec						exitFuncVec_;

		RegistrationMap					tempExactMatchMap_;
		RegistrationVec					tempWildCardVec_;

		RegistrationMap					exactMatchMap_;
		RegistrationVec					wildCardVec_;
	};
}

#endif
