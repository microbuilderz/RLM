#pragma once

#include <OnixS/FIXEngine.h>

namespace RLMEngine
{
	class TakersListener : public OnixS::FIX::ISessionListener
	{
		public:
							TakersListener();
							TakersListener (RLMEngine::RLMlistenerFunctor *ptr);
			virtual 		~TakersListener (void);

			void			setupFunctor(RLMEngine::RLMlistenerFunctor *ptr);

			virtual void	onInboundApplicationMsg (Message& msg, Session* sn);
			virtual void	onInboundSessionMsg (Message& msg, Session* sn);
			virtual void	onStateChange (SessionState::Enum newState, SessionState::Enum prevState, Session* sn);
			virtual void	onError (ErrorReason::Enum reason, const std::string& description, Session* sn);
			virtual void	onWarning (WarningReason::Enum reason, const std::string& description, Session* sn);



		private:
			RLMEngine::RLMlistenerFunctor	*listener_functor;
	};
}
