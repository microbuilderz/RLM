#ifndef SRC_MULTIPLEX_BASE_SERVER_H_
#define SRC_MULTIPLEX_BASE_SERVER_H_

#include "conf.h"

namespace RLMSocketServer
{
	namespace MultiPlex
	{
		class BaseServer
		{
			public:
														BaseServer(int port,RLMServerMode::serverMode mode);
				virtual 								~BaseServer();

				void									Terminate();

				virtual int								RunSocketServer()	= 0;

			protected:

				apr_socket_t							*listenSocket;
				apr_pool_t								*memPool;
				apr_status_t 							retStatus;
				apr_pollset_t							*pollSet;
				apr_int32_t								num;
				const apr_pollfd_t						*ret_pfd;
				SocketStatus							socketStatus;
				RLMSocketServer::ServerState			serverState;

			private:
				int										DEF_LISTEN_PORT;
				int										DEF_SOCKET_BACKLOG;
				RLMServerMode::serverMode				serverMode;
				apr_sockaddr_t							*sa;
		};
	}
}

#endif
