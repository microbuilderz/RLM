#include <iostream>
#include "conf.h"
#include "ClientHandler.h"

#define CRLF_STR					"\r\n"
#define BUFSIZE						4096

namespace RLMClient
{
	// Needs Mutex Locking for read and write, just a quick sample by th3matr1x aka Mehdi
	static void *APR_THREAD_FUNC  read(apr_thread_t *thd, void* data)
	{
		threadHandler *_th= (threadHandler*) data;
		apr_size_t len=BUFSIZE;
		char buf[BUFSIZE];
		while (1)
		{
			if (_th->serverMode==RLMServerMode::SSLMODE )
			{
#ifdef CLIENTSSLMODE
				if (_th->ssl==NULL)
					break;
				int readBytes=SSL_read(_th->ssl,buf,(int)len);
				buf[readBytes] = '\0';
				if (readBytes<=0)
				{
					if (readBytes == 0 && (_th->ssl->shutdown & SSL_RECEIVED_SHUTDOWN))
					{
						len = 0;
						_th->ch->ShutdownSSL(_th->ssl);
						_th->ch->DestroySSL();
						_th->ssl = NULL;
						printf("%sSocket Closed %s\n",FRED,NONE);
						break;
					}
					int i = SSL_get_error(_th->ssl, readBytes);

					switch (i)
					{
						case SSL_ERROR_SYSCALL:
						case SSL_ERROR_SSL:
							_th->ch->ShutdownSSL(_th->ssl);
							_th->ch->DestroySSL();
							_th->ssl = NULL;
							printf("read %sSocket Closed Or SYSCALL error%s\n",FRED,NONE);
						break;
						default:
						break;
					}
					break;
				}
				printf("%s received :%s %s\n",FYELLOW,buf,NONE);
#endif
			}
			else
			{
				apr_status_t rv = apr_socket_recv(_th->ns, buf, &len);
				if (rv == APR_EOF || len == 0)
					break;
				printf("%s received :%s %s\n",FYELLOW,buf,NONE);
			}
		}
		return NULL;
	}

	static void *APR_THREAD_FUNC  write(apr_thread_t *thd, void* data)
	{
		threadHandler *_th= (threadHandler*) data;
		apr_file_t* standard_input = 0;
		apr_file_open_stdin (&standard_input, _th->ch->memPool);
		const char* inputText = (char*)apr_pcalloc (_th->ch->memPool, MAX_INPUT);

		while (1)
		{
			apr_status_t apr_status = apr_file_gets ((char*)inputText, MAX_INPUT, standard_input);
			if (apr_status!= APR_SUCCESS)
			{
				printf("unable to allocate string memory");
				break;
			}
			char *resp_hdr= apr_psprintf(_th->ch->memPool, "%s" CRLF_STR CRLF_STR ,inputText);
			apr_size_t len = strlen(resp_hdr);
			if (_th->serverMode==RLMServerMode::SSLMODE)
			{
#ifdef CLIENTSSLMODE
				if (_th->ssl==NULL)
					break;
				int readBytes=SSL_write(_th->ssl,resp_hdr,(int)len);
				if (readBytes<=0)
				{
					if (readBytes == 0 && (_th->ssl->shutdown & SSL_RECEIVED_SHUTDOWN))
					{
						len = 0;
						_th->ch->ShutdownSSL(_th->ssl);
						_th->ch->DestroySSL();
						_th->ssl = NULL;
						printf("%sSocket Closed %s\n",FRED,NONE);
						break;
					}
					int i = SSL_get_error(_th->ssl, readBytes);

					switch (i)
					{
						case SSL_ERROR_SYSCALL:
						case SSL_ERROR_SSL:
							printf("write %sSocket Closed Or SYSCALL error%s\n",FRED,NONE);
							_th->ch->ShutdownSSL(_th->ssl);
							_th->ch->DestroySSL();
							_th->ssl = NULL;
						break;
						default:
						break;
					}
					break;
				}
#endif
			}
			else
			{
				apr_socket_send(_th->ns,resp_hdr,&len);
			}
		}
		return NULL;
	}

	ClientHandler::ClientHandler(char* hostname,apr_port_t port,int timeoutSec,RLMServerMode::serverMode mode)
	{
		serverMode				= mode;
		listenSocket			= NULL;
		read_Thread					= NULL;
		write_Thread					= NULL;
		_th						= new threadHandler();
		DEF_REMOTE_PORT			= port;
		DEF_REMOTE_HOST			= new char[255];
		DEF_SOCK_TIMEOUT		= (APR_USEC_PER_SEC * timeoutSec);
		if (serverMode==RLMServerMode::SSLMODE)
		{
#ifdef CLIENTSSLMODE
			cSSL				= NULL;
			SSL_ctx				= NULL;
#endif
		}

		strcpy(DEF_REMOTE_HOST,hostname);
		apr_pool_create(&memPool, NULL);
	}

	ClientHandler::~ClientHandler()
	{
		apr_socket_close(listenSocket);
	}

	int	ClientHandler::Connect()
	{
		_th->serverMode = serverMode;
		if (serverMode==RLMServerMode::SSLMODE)
		{
#ifdef CLIENTSSLMODE
			InitializeSSL();
			const SSL_METHOD *SSL_method 	= SSLv23_client_method();
			SSL_ctx 							= SSL_CTX_new( SSL_method );
			cSSL 							= SSL_new(SSL_ctx);
			_th->ssl						= cSSL;
#endif
		}
		apr_sockaddr_t	*sa;
		apr_status_t	rv;

		rv = apr_sockaddr_info_get(&sa, DEF_REMOTE_HOST, APR_INET, DEF_REMOTE_PORT, 0, memPool);
		if (rv != APR_SUCCESS)
			return rv;

		rv = apr_socket_create(&listenSocket, sa->family, SOCK_STREAM, APR_PROTO_TCP, memPool);
		if (rv != APR_SUCCESS)
			return rv;

		apr_socket_opt_set(listenSocket, APR_SO_NONBLOCK, 1);
		apr_socket_timeout_set(listenSocket, DEF_SOCK_TIMEOUT);

		rv = apr_socket_connect(listenSocket, sa);
		if (rv != APR_SUCCESS)
			return rv;

		apr_socket_opt_set(listenSocket, APR_SO_NONBLOCK, 0);
		apr_socket_timeout_set(listenSocket, DEF_SOCK_TIMEOUT);

		_th->ns						= listenSocket;
		_th->ch						= this;
		printf("%s Listening to socket server %p %s\n",FGREEN,listenSocket,NONE);

		return APR_SUCCESS;
	}

	apr_status_t ClientHandler::do_client_task()
	{
		if (serverMode==RLMServerMode::SSLMODE)
		{
#ifdef CLIENTSSLMODE
			apr_os_sock_t  oss;
			apr_status_t							retStatus;
			retStatus =  apr_os_sock_get(&oss, listenSocket);
			if (retStatus != APR_SUCCESS)
				printf("%sSocket oss error%s\n",FRED,NONE);

			SSL_set_fd(cSSL, oss);
			SSL_connect(cSSL);
#endif
		}
		readThread();
		writeThread();

		apr_status_t status;
		apr_thread_join(&status, read_Thread);
		apr_thread_join(&status, write_Thread);

		return APR_SUCCESS;
	}
#ifdef CLIENTSSLMODE
	int ClientHandler::InitializeSSL()
	{

		SSL_load_error_strings();
		SSL_library_init();
		OpenSSL_add_all_algorithms();

		return 0;
	}

	int ClientHandler::DestroySSL()
	{
		ERR_free_strings();
		EVP_cleanup();
		return 0;
	}

	int ClientHandler::ShutdownSSL(SSL* cSSL)
	{
		try
		{
			SSL_shutdown(cSSL);
			SSL_free(cSSL);
			SSL_CTX_free(SSL_ctx);
		}
		catch (...)
		{

		}
        SSL_ctx		= NULL;
		return 0;
	}
#endif
	int ClientHandler::readThread()
	{
		return apr_thread_create(&read_Thread, NULL, read, _th, memPool);
	}

	int ClientHandler::writeThread()
	{
		return apr_thread_create(&write_Thread, NULL, write, _th, memPool);
	}
}
