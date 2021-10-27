#pragma once

#include <RLMconf.hpp>

namespace RLMTest
{
	class MassOrderGenerator
	{
		public:
								MassOrderGenerator(Session *sn);
			virtual				~MassOrderGenerator();

            Message             *GenerateMessage(int i);

			Session				*sn;

            Message             *MassQuote;
            Group               QuoteEntryIDs;
	};
}
