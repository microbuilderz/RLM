#include "inc/engine/adapter/SimpleLPAdapter.h"

namespace RLMEngine
{
    SimpleLPAdapter::SimpleLPAdapter()
    {
        LPInfos.clear();
        LPInfos.push_back(SideInfo(4501,"AXA"));
        LPInfos.push_back(SideInfo(4502,"BNP"));
        LPInfos.push_back(SideInfo(4503,"CIB"));
    }

    SimpleLPAdapter::~SimpleLPAdapter()
    {
    }
}


