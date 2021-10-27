#include "inc/utils/SSLCertificate.h"
#include "multithread/SSLServer.h"

namespace RLMSocketServer
{
	namespace MultiThread
	{
		static int passwordCallback(char *buf, int size, int, void *userData)
		{
			char    *password           = (char*)(userData);
			size_t	password_length		= strlen(password);

			if ((password_length + 1) > size)
			{
				printf("Password specified by environment variable is too big");
				return 0;
			}
			strcpy(buf,password);
			return (int) password_length;
		}

		static void* APR_THREAD_FUNC processConnection(apr_thread_t *thd, void* data)
		{
			threadHandler *_th=(threadHandler*) data;
			int readBytes;
			char buf[BUFSIZE];
			apr_size_t len = sizeof(buf) - 1;
			while (1)
			{
				if ((readBytes = SSL_read(_th->ssl, buf, (int) len)) <= 0)
				{
					int i = SSL_get_error(_th->ssl, readBytes);
					if (readBytes == 0 && (_th->ssl->shutdown & SSL_RECEIVED_SHUTDOWN))
					{
                        printf("Socket Closed\n");
						break;
					}

					switch (i)
					{
						case SSL_ERROR_ZERO_RETURN:;
                            printf("APR EOF\n");
						break;
						case SSL_ERROR_WANT_READ:
						case SSL_ERROR_WANT_WRITE:
							printf("SSL_ERROR_WANT_READ OR WRITE\n");
						break;
						case SSL_ERROR_SYSCALL:
						case SSL_ERROR_SSL:
							printf("SSL_ERROR_SSL or SSL_ERROR_SYSCALL\n");
						break;
						default:
						break;
					}
					return NULL;
				}
				if(readBytes > 0)
				{
					buf[readBytes] = '\0';
					printf(FGREEN "%p :" NONE " len=%d, %s ",_th->ns,readBytes,buf);
				}
			}
			return NULL;
		}

		SSLServer::SSLServer(char* PASS,char* certPath,char* keyPath,apr_port_t port) : MultiThread::BaseServer(port,RLMServerMode::SSLMODE)
		{
			mCertPath				= new char[1024];
			mKeyPath				= new char[1024];

			strcpy(mCertPath,certPath);
			strcpy(mKeyPath,keyPath);

			SSLCertificationPassword	= PASS;
			SSL_ctx					= NULL;
			SSL_method				= NULL;

			InitializeCertification();
		}

		SSLServer::~SSLServer()
		{
		}

		int	SSLServer::RunSocketServer()
		{
			if (serverState==RLMSocketServer::INVALID)
				return 0;
			InitializeSSL();
			SSL_method = SSLv23_server_method();
			SSL_ctx = SSL_CTX_new( SSL_method );

			SSL_CTX_set_options(SSL_ctx, SSL_OP_SINGLE_DH_USE);

			while (1)
			{
				apr_socket_t *ns;

				retStatus = apr_socket_accept(&ns, listenSocket, memPool);
				if (retStatus != APR_SUCCESS)
				{
					BaseServer::Terminate();
					break;
				}
				apr_socket_opt_set(ns, APR_SO_NONBLOCK, 0);
				apr_socket_timeout_set(ns, -1);

				int use_cert	= SSL_CTX_use_certificate_file(SSL_ctx, mCertPath , SSL_FILETYPE_PEM);
				if (use_cert<=0)
				{
					printf("unable to use certificate file ");
					break;
				}
                SSL_CTX_set_default_passwd_cb(SSL_ctx, passwordCallback);
				SSL_CTX_set_default_passwd_cb_userdata(SSL_ctx,(void*)(SSLCertificationPassword.c_str()));
				int use_prv		= SSL_CTX_use_PrivateKey_file(SSL_ctx, mKeyPath, SSL_FILETYPE_PEM);
				if (use_prv<=0)
				{
					printf("unable to use private key file ");
					break;
				}

				cSSL = SSL_new(SSL_ctx);

				apr_os_sock_t  oss;
				retStatus =  apr_os_sock_get(&oss, ns);
				if(retStatus != APR_SUCCESS)
				{
					printf("Error getting OS socket fd\n");
					break;
				}
				SSL_set_fd(cSSL, oss);
				if (retStatus != APR_SUCCESS)
				{
					BaseServer::Terminate();
					break;
				}
				int ssl_err = SSL_accept(cSSL);
				if (ssl_err<1)
				{
					CheckError(ssl_err,cSSL);
					ShutdownSSL();
					BaseServer::Terminate();
					break;
				}

				apr_thread_t *thd_obj;

				threadHandler* _th=new threadHandler();
				_th->ns=ns;
				_th->ssl=cSSL;

				printf("new client " FBLUE "%p" NONE "\n",ns);

				retStatus = apr_thread_create(&thd_obj, NULL, processConnection, _th, memPool);

				if(retStatus != APR_SUCCESS)
					printf("Error Creating new Thread\n");
			}
			return 0;
		}

		int SSLServer::InitializeSSL()
		{
			SSL_load_error_strings();
			SSL_library_init();
			OpenSSL_add_all_algorithms();
			return 0;
		}

		int SSLServer::ShutdownSSL()
		{
			SSL_shutdown(cSSL);
			SSL_free(cSSL);
			return 0;
		}

		int SSLServer::DestroySSL()
		{
			ERR_free_strings();
			EVP_cleanup();
			return 0;
		}

		int SSLServer::InitializeCertification()
		{
			SSLCertificate mCert;
			mCert.generateCertification(SSLCertificationPassword,mKeyPath,mCertPath);
			mCert.cleanUp();
			return 0;
		}

		void SSLServer::CheckError(int error,SSL* cSSL)
		{
			int i = SSL_get_error(cSSL, error);
			printf("SSL error code = %d",i);
			char* SSL_ErrorDumper =ERR_error_string(ERR_get_error(), NULL);
			printf("SSL error dumper = %s",SSL_ErrorDumper);
		}
	}
}
