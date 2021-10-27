#define DEF_POLL_SET_NUM 32

#include "multiplex/BaseServer.h"

namespace RLMSocketServer
{
	namespace MultiPlex
	{
		BaseServer::BaseServer(int port,RLMServerMode::serverMode mode)
		{
			apr_initialize();
			serverState				= RLMSocketServer::VALID;
			DEF_SOCKET_BACKLOG		= SOMAXCONN;
			DEF_LISTEN_PORT			= port;
			serverMode				= mode;
			serverMode				== RLMServerMode::SSLMODE ? printf("MultiPlex SSLMode\n") : printf("MultiPlex No SSLMode\n");
			apr_pool_create(&memPool, NULL);

			retStatus = apr_sockaddr_info_get(&sa, NULL, APR_INET, (apr_port_t) DEF_LISTEN_PORT, 0, memPool);
			if (retStatus != APR_SUCCESS)
			{
				Terminate();
				return;
			}

			retStatus = apr_socket_create(&listenSocket, sa->family, SOCK_STREAM, APR_PROTO_TCP, memPool);
			if (retStatus != APR_SUCCESS)
			{
				Terminate();
				return;
			}

			apr_socket_opt_set(listenSocket, APR_SO_NONBLOCK, 1);
			apr_socket_timeout_set(listenSocket, 0);
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

			retStatus = apr_pollset_create(&pollSet, DEF_POLL_SET_NUM, memPool, 0);
			if (retStatus != APR_SUCCESS)
			{
				Terminate();
				return;
			}
			apr_pollfd_t pfd = { memPool, APR_POLL_SOCKET, APR_POLLIN, 0, { NULL }, NULL };
			pfd.desc.s = listenSocket;
			retStatus = apr_pollset_add(pollSet, &pfd);
			if (retStatus != APR_SUCCESS)
			{
				Terminate();
				return;
			}
		}

		BaseServer::~BaseServer()
		{
			apr_pollset_destroy(pollSet);
			apr_pool_destroy(memPool);
			apr_terminate();
		}

		void BaseServer::Terminate()
		{
			serverState	= RLMSocketServer::INVALID;
			char errbuf[256];
			apr_strerror(retStatus, errbuf, sizeof(errbuf));
			printf("error: %d, %s\n", retStatus, errbuf);
		}
	}
}
