#include "RLMconf.hpp"
#include "engine/makers/MakersListener.h"

namespace RLMEngine
{
	MakersListener::MakersListener()
	{
		listener_functor=NULL;
	}

	void MakersListener::setupFunctor(RLMEngine::RLMlistenerFunctor *ptr)
	{
		listener_functor=ptr;
		RLMListenerStruct _struct;
		_struct.sn	= NULL;
		_struct.msg = NULL;
		_struct.listener    = NULL;
		listener_functor->onInit(&_struct);
	}

	MakersListener::MakersListener(RLMEngine::RLMlistenerFunctor *ptr)
	{
		listener_functor=ptr;
		RLMListenerStruct _struct;
		_struct.sn	= NULL;
		_struct.msg = NULL;
		_struct.listener    = NULL;
		listener_functor->onInit(&_struct);
	}

	MakersListener::~MakersListener()
	{
	}

	void MakersListener::onInboundApplicationMsg (Message& msg, Session* sn)
	{
		RLMListenerStruct _struct;
		_struct.sn	= sn;
		_struct.msg = &msg;
		_struct.listener    = NULL;
		listener_functor->onInboundApplicationMsg(&_struct);
	}

	void MakersListener::onInboundSessionMsg (Message& msg, Session* sn)
	{
		cout	<<FCYAN<< "\nIncoming session-level message:\n" << NONE<< msg << endl;
		RLMListenerStruct _struct;
		_struct.sn	= sn;
		_struct.msg = &msg;
		_struct.listener    = NULL;
		listener_functor->onInboundSessionMsg(&_struct);
	}

	void MakersListener::onStateChange (SessionState::Enum newState, SessionState::Enum prevState, Session* sn)
	{
		cout	<<FYELLOW<< "\nSession's state is changed, prevState=" << SessionState::toString (prevState)
				<< ", newState=" << SessionState::toString (newState) << NONE<< endl;
		RLMListenerStruct _struct;
		_struct.sn	= sn;
		_struct.msg = NULL;
		_struct.listener    = NULL;
		listener_functor->onStateChangePtr(&_struct);
	}

	void MakersListener::onError (ErrorReason::Enum reason, const std::string& description, Session* sn)
	{
		cerr	<<FRED<< "\nSession-level error:" << description << NONE<< endl;
	}

	void MakersListener::onWarning (WarningReason::Enum reason, const std::string& description, Session* sn)
	{
		cerr	<<FRED<< "\nSession-level warning:" << description << NONE<< endl;
	}
}


