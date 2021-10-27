#include <unistd.h>
#include <iostream>
#include <apr-1.0/apr.h>

#include "pluginFramework/PluginManager.h"
#include "utils/Path.h"
#include "utils/Directory.h"

namespace RLMFrameWork
{
	static std::string				dynamicLibraryExtension("so");
	static bool						isValid(const unsigned char * objectType, const PF_RegisterParams * params)
	{
		if (!objectType || !(*objectType))
			return false;
		if (!params ||!params->createFunc || !params->destroyFunc)
			return false;

		return true;
	}

	PluginManager::PluginManager() : inInitializePlugin_(false)
	{
		platformServices_.version.major		= 1;
		platformServices_.version.minor		= 0;
		platformServices_.invokeService		= NULL;
		platformServices_.registerObject	= registerObject;
	}

	PluginManager::~PluginManager()
	{
		shutdown();
	}

	int PluginManager::registerObject(const unsigned char *objectType, const PF_RegisterParams *params)
	{
		if (!isValid(objectType, params))
			return -1;

		PluginManager & pm = PluginManager::getInstance();

		PF_PluginAPI_Version v = pm.platformServices_.version;
		if (v.major != params->version.major)
        {
            printf("wrong versioning !!! \n");
            return -1;
        }

		std::string key((const char *)objectType);
		if (key == std::string("*"))
		{
			pm.wildCardVec_.push_back(*params);
			return 0;
		}
		if (pm.exactMatchMap_.find(key) != pm.exactMatchMap_.end()) {
            printf("Exact match up !!! \n");
            return -1;
        }

		pm.exactMatchMap_[key] = *params;
		return 0;
	}

	PluginManager & PluginManager::getInstance()
	{
		static PluginManager instance;
		return instance;
	}


	int PluginManager::loadAll(const std::string & pluginDirectory, PF_InvokeServiceFunc func)
	{
		if (pluginDirectory.empty())
			return -1;

		platformServices_.invokeService = func;

		Path dir_path(pluginDirectory);
		if (!dir_path.exists() || !dir_path.isDirectory())
			return -1;

		Directory::Entry e;
		Directory::Iterator di(dir_path);
		while (di.next(e))
		{
			Path full_path(dir_path + Path(e.path));

			if (full_path.isDirectory())
			continue;

			std::string ext = std::string(full_path.getExtension());
			if (ext != dynamicLibraryExtension)
			continue;

			printf("loading %s\n",std::string(full_path).c_str());
			int res=loadByPath(std::string(full_path));

			if (res>=0)
				printf("loaded\n");
		}

		return 0;
	}

	int PluginManager::initializePlugin(PF_InitFunc initFunc)
	{
		PluginManager & pm = PluginManager::getInstance();

		PF_ExitFunc exitFunc = initFunc(&pm.platformServices_);
		if (!exitFunc)
			return -1;

		pm.exitFuncVec_.push_back(exitFunc);
		return 0;
	}

	int PluginManager::shutdown()
	{
		int result = 0;
		for (ExitFuncVec::iterator func = exitFuncVec_.begin(); func != exitFuncVec_.end(); ++func)
		{
			try
			{
				result = (*func)();
			}
			catch (...)
			{
				result = -1;
			}
		}

		dynamicLibraryMap_.clear();
		exactMatchMap_.clear();
		wildCardVec_.clear();
		exitFuncVec_.clear();

		return result;
	}

	int PluginManager::loadByPath(const std::string & pluginPath)
	{
		Path path(pluginPath);

		if (path.isSymbolicLink())
		{
			char buff[APR_PATH_MAX+1];
			int length = ::readlink(path, buff, APR_PATH_MAX);
			if (length < 0)
				return -1;

			path = std::string(buff, length);
		}
		// dynamic lib already loaded
		if (dynamicLibraryMap_.find(std::string(path)) != dynamicLibraryMap_.end())
		{
			printf("library already loaded\n");
			return -1;
		}

		path.makeAbsolute();

		std::string errorString;
		DynamicLibrary * d = loadLibrary(std::string(path), errorString);
		if (!d)
			return -1;
		PF_InitFunc initFunc = (PF_InitFunc)(d->getSymbol("PF_initPlugin"));
		if (!initFunc)
			return -1;
		int res = initializePlugin(initFunc);
		if (res < 0)
			return res;
		return 0;
	}

	void * PluginManager::createObject(const std::string & objectType, IObjectAdapter & adapter)
	{
		if (objectType == std::string("*"))
			return NULL;

		PF_ObjectParams np;
		np.objectType = (const unsigned char *)objectType.c_str();
		np.platformServices = &platformServices_;

		if (exactMatchMap_.find(objectType) != exactMatchMap_.end())
		{
			PF_RegisterParams & rp = exactMatchMap_[objectType];
			void * object = rp.createFunc(&np);
			if (object)
			{
				if (rp.programmingLanguage == PF_ProgrammingLanguage_C)
					object = adapter.adapt(object, rp.destroyFunc);
				return object;
			}
		}

		for (size_t i = 0; i < wildCardVec_.size(); ++i)
		{
			PF_RegisterParams & rp = wildCardVec_[i];
			void * object = rp.createFunc(&np);
			if (object)
			{
				if (rp.programmingLanguage == PF_ProgrammingLanguage_C)
					object = adapter.adapt(object, rp.destroyFunc);
				int res = registerObject(np.objectType, &rp);
				if (res < 0)
				{
					rp.destroyFunc(object);
					return NULL;
				}
				return object;
			}
		}
		return NULL;
	}

	RLMFrameWork::DynamicLibrary * PluginManager::loadLibrary(const std::string &  path, std::string & errorString)
	{
		DynamicLibrary * d = DynamicLibrary::load(path, errorString);
		if (!d)
			return NULL;
		dynamicLibraryMap_[Path::makeAbsolute(path)] = d;
		return d;
	}

	const PluginManager::RegistrationMap & PluginManager::getRegistrationMap()
	{
		return exactMatchMap_;
	}

	PF_PlatformServices & PluginManager::getPlatformServices()
	{
		return platformServices_;
	}
}
