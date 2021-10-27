#ifndef SRC_BASE_SERVER_HANDLER_H_
#define SRC_BASE_SERVER_HANDLER_H_

#include "conf.h"

namespace RLMSocketServer
{
	namespace MultiThread
	{
		class BaseServer
		{
		public:
															BaseServer(apr_port_t port,RLMServerMode::serverMode mode);
			virtual											~BaseServer();

			void											Terminate();

			virtual int										RunSocketServer()	= 0;

		protected:

			apr_socket_t									*listenSocket;
			apr_pool_t										*memPool;
			apr_status_t 									retStatus;
			RLMSocketServer::ServerState					serverState;

		private:
			apr_port_t										DEF_LISTEN_PORT;
			int												DEF_SOCKET_BACKLOG;
			RLMServerMode::serverMode						serverMode;

			apr_threadattr_t								*thd_attr;
			apr_sockaddr_t									*sa;
		};
	}
}

#endif
