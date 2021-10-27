#include "multithread/BaseServer.h"

namespace RLMSocketServer
{
	namespace MultiThread
	{
		BaseServer::BaseServer(apr_port_t port,RLMServerMode::serverMode mode)
		{
			apr_initialize();
			serverState				= RLMSocketServer::VALID;
			DEF_SOCKET_BACKLOG		= SOMAXCONN;
			DEF_LISTEN_PORT			= port;
			serverMode				= mode;
			serverMode				== RLMServerMode::SSLMODE ? printf("MultiThread SSLMode\n") : printf("MultiThread No SSLMode\n");
			apr_pool_create(&memPool, NULL);
			apr_threadattr_create(&thd_attr, memPool);

			retStatus = apr_sockaddr_info_get(&sa, NULL, APR_INET, DEF_LISTEN_PORT, 0, memPool);
			if (retStatus != APR_SUCCESS)
			{
				Terminate();
				return;
			}

			retStatus = apr_socket_create(&listenSocket, sa->family, SOCK_STREAM, APR_PROTO_TCP, memPool);
			if (retStatus != APR_SUCCESS)
			{
				Terminate();
				return ;
			}

			apr_socket_opt_set(listenSocket, APR_SO_NONBLOCK, 0);
			apr_socket_timeout_set(listenSocket, -1);
			apr_socket_opt_set(listenSocket, APR_SO_REUSEADDR, 1);

			retStatus = apr_socket_bind(listenSocket, sa);
			if (retStatus != APR_SUCCESS)
			{
				Terminate();
				return;
			}
			retStatus = apr_socket_listen(listenSocket, DEF_SOCKET_BACKLOG);
			if (retStatus != APR_SUCCESS)
			{
				Terminate();
				return;
			}
		}

		BaseServer::~BaseServer()
		{
			apr_pool_destroy(memPool);
			apr_terminate();
		}

		void BaseServer::Terminate()
		{
			serverState	= RLMSocketServer::INVALID;
			char errorBuffer[256];
			apr_strerror(retStatus, errorBuffer, sizeof(errorBuffer));
			printf("error: %d, %s\n", retStatus, errorBuffer);
		}
	}
}
