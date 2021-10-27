#pragma once

#include <iostream>
#include <vector>
#include "RLMconf.hpp"

namespace RLMEngine
{
    class SimpleLPAdapter
    {
        public:
                                                SimpleLPAdapter();
            virtual                             ~SimpleLPAdapter();

            std::vector<RLMEngine::SideInfo>    getLPInfos() { return LPInfos; }

        private:
            std::vector<RLMEngine::SideInfo>    LPInfos;
    };
}

