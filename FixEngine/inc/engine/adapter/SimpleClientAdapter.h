#pragma once

#include <iostream>
#include <vector>
#include "RLMconf.hpp"

namespace RLMEngine {
    class SimpleClientAdapter
    {
        public:
                                                SimpleClientAdapter();
            virtual                             ~SimpleClientAdapter();

            std::vector<std::string>            getClientInfos()    { return Infos; }

        private:
            std::vector<std::string>            Infos;
    };
}

