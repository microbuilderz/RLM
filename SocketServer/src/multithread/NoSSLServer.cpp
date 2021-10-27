#include "multithread/NoSSLServer.h"

namespace RLMSocketServer
{
	namespace MultiThread
	{
		static void* APR_THREAD_FUNC processConnection(apr_thread_t *thd, void* data)
		{
			apr_socket_t * sock = (apr_socket_t*) data;
			while (1)
			{
				char buf[BUFSIZE];
				apr_size_t len = sizeof(buf) - 1;
				apr_status_t rv = apr_socket_recv(sock, buf, &len);

				if (rv == APR_EOF || len == 0)
				{
					printf("Socket Closed\n");
					apr_socket_close(sock);
					break;
				}
				if(len > 0)
				{
					buf[len] = '\0';
					printf("Read: %s\n", buf);
				}
			}
			return NULL;
		}

		NoSSLServer::NoSSLServer(apr_port_t port) : MultiThread::BaseServer(port,RLMServerMode::NOSSLMODE)
		{
		}

		NoSSLServer::~NoSSLServer()
		{
		}

        // TODO add socket termination
		int	NoSSLServer::RunSocketServer()
		{
			if (serverState==RLMSocketServer::INVALID)
				return 0;
			while (1)
			{
				apr_socket_t *ns;

				retStatus = apr_socket_accept(&ns, listenSocket, memPool);
				if (retStatus != APR_SUCCESS) {
					BaseServer::Terminate();
					break;
				}

				apr_socket_opt_set(ns, APR_SO_NONBLOCK, 0);
				apr_socket_timeout_set(ns, -1);

				apr_thread_t *thd_obj;
				retStatus = apr_thread_create(&thd_obj, NULL, processConnection, ns, memPool);

				if(retStatus != APR_SUCCESS)
					printf("Error Creating new Thread\n");
			}
            return 0;
        }
	}
}
