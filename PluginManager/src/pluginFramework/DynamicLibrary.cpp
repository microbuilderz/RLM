#include <dlfcn.h>
#include <iostream>
#include <sstream>

#include "pluginFramework/DynamicLibrary.h"

namespace RLMFrameWork
{
	DynamicLibrary::DynamicLibrary(void * handle) : handle_(handle)
	{
	}

	DynamicLibrary::~DynamicLibrary()
	{
		if (handle_)
			::dlclose(handle_);
	}

	DynamicLibrary *DynamicLibrary::load(const std::string & name, std::string & errorString)
	{
		if (name.empty())
		{
			errorString = "Empty path.";
			return NULL;
		}

		void * handle = NULL;

		handle = ::dlopen(name.c_str(), RTLD_NOW);
		if (!handle)
		{
			std::string dlErrorString;
			const char *zErrorString = ::dlerror();
			if (zErrorString)
				dlErrorString = zErrorString;
			errorString += "Failed to load \"" + name + '"';
			if(dlErrorString.size())
				errorString += ": " + dlErrorString;
			return NULL;
		}

		return new DynamicLibrary(handle);
	}

	void *DynamicLibrary::getSymbol(const std::string & symbol)
	{
		if (!handle_)
			return NULL;

		return ::dlsym(handle_, symbol.c_str());
	}
}

