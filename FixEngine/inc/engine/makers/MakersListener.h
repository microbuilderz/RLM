#pragma once

#include <OnixS/FIXEngine.h>

namespace RLMEngine
{
	/*!
	 * \brief listen for makers state changes, inbound application,
	 * session , error and warning messages.
	 *
	 * all Makers sessions should implement this listener.
	 */
	class MakersListener : public OnixS::FIX::ISessionListener
	{
		public:
							MakersListener();
							MakersListener(RLMEngine::RLMlistenerFunctor *ptr);
			virtual 		~MakersListener();

			void			setupFunctor(RLMEngine::RLMlistenerFunctor *ptr);

			/**
			   * listener for inbound application messages.
			   * @param msg : OnixS Fix Message.
			   * @param sn  : OnixS Session.
			   * @see http://ref.onixs.biz/cpp-fix-engine-guide
			*/
			virtual void	onInboundApplicationMsg (Message& msg, Session* sn);

			/**
			   * listener for inbound session messages.
			   * @param msg : OnixS Fix Message.
			   * @param sn  : OnixS Session.
			   * @see http://ref.onixs.biz/cpp-fix-engine-guide
			*/
			virtual void	onInboundSessionMsg (Message& msg, Session* sn);

			/**
			   * listener for session state changes.
			   * @param newState : OnixS new session state.
			   * @param prevState: OnixS previous session state.
			   * @param sn  : OnixS Session.
			   * @see http://ref.onixs.biz/cpp-fix-engine-guide
			*/
			virtual void	onStateChange (SessionState::Enum newState, SessionState::Enum prevState, Session* sn);

			/**
			   * listener for session errors.
			   * @param reason 		: OnixS error reason value.
			   * @param description	: OnixS error description.
			   * @param sn  		: OnixS Session.
			   * @see http://ref.onixs.biz/cpp-fix-engine-guide
			*/
			virtual void	onError (ErrorReason::Enum reason, const std::string& description, Session* sn);

			/**
			   * listener for session warnings.
			   * @param reason 		: OnixS warning reason value.
			   * @param description	: OnixS warning description.
			   * @param sn  		: OnixS Session.
			   * @see http://ref.onixs.biz/cpp-fix-engine-guide
			*/
			virtual void	onWarning (WarningReason::Enum reason, const std::string& description, Session* sn);

		private:
			RLMEngine::RLMlistenerFunctor	*listener_functor;	///< holds a structure of function pointers to be used by the session listener \ref RLMEngine::RLMlistenerFunctor
	};
}
