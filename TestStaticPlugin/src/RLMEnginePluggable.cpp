#include <string.h>
#include <iostream>

#include "RLMEnginePluggable.h"

using namespace std;

void *RLMEnginePluggable::create(PF_ObjectParams *)
{
	RLMEnginePluggable* a=new RLMEnginePluggable();
	return a;
}

int RLMEnginePluggable::destroy(void * p)
{
	if (!p)
		return -1;
	delete (RLMEnginePluggable *)p;
	return 0;
}

RLMEnginePluggable::RLMEnginePluggable()
{
}

RLMEnginePluggable::~RLMEnginePluggable()
{
}

void RLMEnginePluggable::getInitialInfo(PluginInfo * info)
{
	::strcpy((char *)info->name, "RLMEnginePluggable");
	info->id = 0;
}

void RLMEnginePluggable::run()
{
	cout<<"running RLMEnginePluggable"<<endl;
}
