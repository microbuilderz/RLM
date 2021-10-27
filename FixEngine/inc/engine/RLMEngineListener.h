#pragma once

#include "RLMconf.hpp"

namespace RLMEngine
{
	class RLMEngineListener : public OnixS::FIX::IEngineListener
	{
		public:
									RLMEngineListener();
									~RLMEngineListener();
			void					onUnknownIncomingConnection(const Message& incomingLogon,
																const int listenPort,
																const int counterPartyPort,
																const std::string& counterPartyIpAddress,
																std::string* rejectReason);
			void					onError (EngineErrorReason::Enum reason, const std::string& description);
			void					onWarning (EngineWarningReason::Enum reason, const std::string& description);
	};
}
