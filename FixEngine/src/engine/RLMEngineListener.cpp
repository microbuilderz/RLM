#include <engine/RLMEngineListener.h>

namespace RLMEngine
{
	RLMEngineListener::RLMEngineListener()
	{
	}

	RLMEngineListener::~RLMEngineListener()
	{
	}

	void RLMEngineListener::onUnknownIncomingConnection (const Message& incomingLogon, const int listenPort, const int counterPartyPort, const std::string& counterPartyIpAddress, std::string* rejectReason)
	{
	};

	void RLMEngineListener::onError (EngineErrorReason::Enum reason, const std::string& description)
	{
		std::cerr << "\nEngine-level error: " << description << endl;
	}

	void RLMEngineListener::onWarning (EngineWarningReason::Enum reason, const std::string& description)
	{
		std::cerr << "\nEngine-level warning: " << description << endl;
	}
}
