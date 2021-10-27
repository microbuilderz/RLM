#include "inc/engine/adapter/SimpleClientAdapter.h"

namespace RLMEngine
{
    SimpleClientAdapter::SimpleClientAdapter()
    {
        Infos.clear();
        Infos.push_back("Trader1");
        Infos.push_back("Trader2");
        Infos.push_back("Trader3");
    }

    SimpleClientAdapter::~SimpleClientAdapter()
    {
    }
}
