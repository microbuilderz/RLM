#include "RLMconf.hpp"
#include "engine/takers/TakersListener.h"

namespace RLMEngine
{
	TakersListener::TakersListener()
	{
		listener_functor	= NULL;
	}
	TakersListener::TakersListener(RLMEngine::RLMlistenerFunctor *ptr)
	{
		listener_functor	=ptr;
		RLMListenerStruct _struct;
		_struct.sn			= NULL;
		_struct.msg 		= NULL;
        _struct.listener    = NULL;
		listener_functor->onInit(&_struct);
	}

	TakersListener::~TakersListener()
	{
	}

	void TakersListener::setupFunctor(RLMEngine::RLMlistenerFunctor *ptr)
	{
		listener_functor=ptr;
		RLMListenerStruct _struct;
		_struct.sn	        = NULL;
		_struct.msg         = NULL;
        _struct.listener    = NULL;
		listener_functor->onInit(&_struct);
	}

	void TakersListener::onInboundApplicationMsg (Message& msg, Session* sn)
	{
		RLMListenerStruct _struct;
		_struct.sn	        = sn;
		_struct.msg         = &msg;
        _struct.listener    = NULL;
		listener_functor->onInboundApplicationMsg(&_struct);
	}

	void TakersListener::onInboundSessionMsg (Message& msg, Session* sn)
	{
		cout <<FPURPLE <<"\nTakers Incoming session-level message:\n" <<NONE << msg << endl;
		RLMListenerStruct _struct;
		_struct.sn	        = sn;
		_struct.msg         = &msg;
        _struct.listener    = this;
		listener_functor->onInboundSessionMsg(&_struct);
	}

	void TakersListener::onStateChange (SessionState::Enum newState, SessionState::Enum prevState, Session* sn)
	{
		cout <<FYELLOW<< "\nTakers Session's state is changed, prevState=" << SessionState::toString (prevState)
			 << ", newState=" << SessionState::toString (newState) <<NONE << endl;
		RLMListenerStruct _struct;
		_struct.sn	        = sn;
		_struct.msg         = NULL;
        _struct.listener    = NULL;
		listener_functor->onStateChangePtr(&_struct);
	}

	void TakersListener::onError (ErrorReason::Enum reason, const std::string& description, Session* sn)
	{
		cerr <<FRED << "\nSession-level error:" << description <<NONE<< endl;
	}

	void TakersListener::onWarning (WarningReason::Enum reason, const std::string& description, Session* sn)
	{
		cerr <<FRED<< "\nSession-level warning:" << description <<NONE<< endl;
	}
}
