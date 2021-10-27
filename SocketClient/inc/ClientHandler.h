#ifndef CLIENT_HANDLER_H_
#define CLIENT_HANDLER_H_

#include "conf.h"

namespace RLMClient
{
	class ClientHandler
	{
	public:
												ClientHandler(char* hostname=(char*)ENGINEHOSTNAME,apr_port_t port=8081,int timeoutSec=30,RLMServerMode::serverMode mode=RLMServerMode::SSLMODE);
		virtual 								~ClientHandler();

		int										Connect();
		apr_status_t do_client_task();
#ifdef CLIENTSSLMODE
		int										DestroySSL();
		int										ShutdownSSL(SSL* cSSL);
#endif

		apr_pool_t								*memPool;
	private:
#ifdef CLIENTSSLMODE
		int										InitializeSSL();
#endif
		int										readThread();
		int										writeThread();

		apr_port_t								DEF_REMOTE_PORT;
		char*									DEF_REMOTE_HOST;
		long									DEF_SOCK_TIMEOUT;

		apr_socket_t							*listenSocket;

		RLMServerMode::serverMode				serverMode;
#ifdef CLIENTSSLMODE
		SSL 									*cSSL;
		SSL_CTX									*SSL_ctx;
#endif
		apr_thread_t 							*read_Thread;
		apr_thread_t 							*write_Thread;
		threadHandler							*_th;
	};
}

#endif
