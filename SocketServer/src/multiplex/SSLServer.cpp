#include "multiplex/SSLServer.h"
#include "utils/SSLCertificate.h"

#define DEF_POLL_TIMEOUT	(APR_USEC_PER_SEC * 30)

namespace RLMSocketServer
{
	namespace MultiPlex
	{
		static int passwd_cb(char *buf,int size,int rwflag,void *userdata)
		{
			char				*password = (char*)(userdata);
			int					password_length;

			password_length		= (int) strlen(password);

			if ((password_length + 1) > size)
			{
				printf("%sPassword specified by environment variable is too big%s",FRED,NONE);
				return 0;
			}
			strcpy(buf,password);
			return password_length;
		}

		SSLServer::SSLServer(char* PASS,char* certPath,char* keyPath,int port) : MultiPlex::BaseServer(port,RLMServerMode::SSLMODE)
		{
			mCertPath				= new char[1024];
			mKeyPath				= new char[1024];

			strcpy(mCertPath,certPath);
			strcpy(mKeyPath,keyPath);

			SSLCertificationPassword	= PASS;
			SSL_ctx					= NULL;
			SSL_method				= NULL;
			response_handler		= NULL;

			InitializeCertification();
		}

		SSLServer::~SSLServer()
		{
		}

		void SSLServer::SetReadHandler(int (*fp)(void*))
		{
			response_handler = fp;
		}

		int	SSLServer::RunSocketServer()
		{
			if (serverState==RLMSocketServer::INVALID)
				return 0;
			InitializeSSL();
			SSL_method = SSLv23_server_method();
			SSL_ctx = SSL_CTX_new( SSL_method );

			SSL_CTX_set_options(SSL_ctx, SSL_OP_SINGLE_DH_USE);

			// TODO add termination handler
			while (1)
			{
				retStatus = apr_pollset_poll(pollSet, DEF_POLL_TIMEOUT, &num, &ret_pfd);
				if (retStatus == APR_SUCCESS)
				{
					int i;
					for (i = 0; i < num; i++)
					{
						if (ret_pfd[i].desc.s == listenSocket)
							PollAccept();
						else
						{
							SSLServCTX *sslCtx = (SSLServCTX *)ret_pfd[i].client_data;
							((this)->*socket_callback_t)(sslCtx);
						}
					}
				}
			}
			return 0;
		}

		int	SSLServer::PollAccept()
		{
			apr_socket_t *ns;

			retStatus = apr_socket_accept(&ns, listenSocket, memPool);

			if (retStatus != APR_SUCCESS)
			{
				BaseServer::Terminate();
				return 0;
			}

			int use_cert	= SSL_CTX_use_certificate_file(SSL_ctx, mCertPath , SSL_FILETYPE_PEM);
			if (use_cert<=0)
			{
				printf("%sunable to use certificate file%s\n",FRED,NONE);
			}
			SSL_CTX_set_default_passwd_cb(SSL_ctx,passwd_cb);
			SSL_CTX_set_default_passwd_cb_userdata(SSL_ctx,(void*)(SSLCertificationPassword.c_str()));
			int use_prv		= SSL_CTX_use_PrivateKey_file(SSL_ctx, mKeyPath, SSL_FILETYPE_PEM);
			if (use_prv<=0)
				printf("%sunable to use private key file%s\n",FRED,NONE);

			SSL* cSSL = SSL_new(SSL_ctx);

			apr_os_sock_t  oss;
			retStatus =  apr_os_sock_get(&oss, ns);
			if(retStatus != APR_SUCCESS)
				printf("%sError getting OS socket fd%s\n",FRED,NONE);
			SSL_set_fd(cSSL, oss);
		accept:
			int ssl_err = SSL_accept(cSSL);
			if (ssl_err<1)
			{
				int error = SSL_get_error(cSSL, ssl_err);
				if (error==SSL_ERROR_WANT_READ || error==SSL_ERROR_WANT_WRITE)
				{
					apr_sleep(10000);
					goto accept;
				}

				CheckError(ssl_err,cSSL);
				ShutdownSSL(cSSL);
				BaseServer::Terminate();
				return 0;
			}
			if (retStatus == APR_SUCCESS)
			{
				_SSL_ctx_t* cSSLCtx	= new _SSL_ctx_t();
				cSSLCtx->sock		= ns;
				cSSLCtx->sslSock	= cSSL;
				retStatus =  apr_os_sock_get(&oss, ns);
				if(retStatus != APR_SUCCESS)
					printf("%sError getting OS socket fd%s\n",FRED,NONE);
				printf("new client connected %s%p%s\n",FYELLOW,cSSL,NONE);
				apr_pollfd_t pfd = { memPool, APR_POLL_SOCKET, APR_POLLIN, 0, { NULL }, cSSLCtx };
				pfd.desc.s = ns;
				apr_socket_opt_set(ns, APR_SO_NONBLOCK, 1);
				apr_socket_timeout_set(ns, 0);
				socketStatus 		= SERV_RECV_REQUEST;
				socket_callback_t	= &SSLServer::ReceiveRequestCallBack;
				apr_pollset_add(pollSet, &pfd);
			}
			return true;
		}

		int	SSLServer::ReceiveRequestCallBack(SSLServCTX *sslCtx)
		{
			char buf[BUFSIZE];
			apr_size_t len = sizeof(buf) - 1;
			while (1)
			{
				int readbytes=SSL_read(sslCtx->sslSock, buf, (int) len);
				if (readbytes<=0)
				{
					apr_pollfd_t pfd		= { memPool, APR_POLL_SOCKET, APR_POLLOUT, 0, { NULL }, sslCtx };
					if (readbytes == 0 && (sslCtx->sslSock->shutdown & SSL_RECEIVED_SHUTDOWN))
					{
                        ShutdownSSL(sslCtx->sslSock);
						pfd.desc.s				= sslCtx->sock;
						apr_pollset_remove(pollSet, &pfd);
						apr_socket_close(sslCtx->sock);
						printf("Socket Closed\n");
						break;
					}


					int i = SSL_get_error(sslCtx->sslSock, readbytes);

					switch (i)
					{
						case SSL_ERROR_WANT_WRITE:
							printf("%sSSL read error , server might not be able to respond anymore%s\n",FRED,NONE);
						break;
						case SSL_ERROR_SYSCALL:
						case SSL_ERROR_SSL:
							ShutdownSSL(sslCtx->sslSock);
							pfd.desc.s				= sslCtx->sock;
							apr_pollset_remove(pollSet, &pfd);
							apr_socket_close(sslCtx->sock);
						break;
						default:
						break;
					}

					break;
				}

				if (len==0)
					break;

				if (strstr(buf, "GET"))
				{
					char **tokens;
					apr_tokenize_to_argv(buf, &tokens, memPool);
					if (tokens[0] && tokens[1] && strcmp(tokens[0], "GET") == 0)
					{
						sslCtx->recv.filePath = apr_pstrdup(memPool, tokens[1]);
					}
				}
				if (strstr(buf, CRLF_STR CRLF_STR) && strstr(buf, "GET") )
				{
					socketStatus				= SERV_SEND_RESPONSE;
					socket_callback_t 			= &SSLServer::SendResponseCallBack;
					apr_pollfd_t pfd 			= { memPool, APR_POLL_SOCKET, APR_POLLIN, 0, { NULL }, sslCtx };
					pfd.desc.s 					= sslCtx->sock;
					apr_pollset_remove(pollSet, &pfd);
					pfd.reqevents 				= APR_POLLOUT;
					apr_pollset_add(pollSet, &pfd);
					sslCtx->send.send_buf		= NULL;

					break;
				}
				buf[readbytes] = '\0';
				buf[strcspn(buf, "\r\n")] = '\0';
				printf("%s%p:%s%s\n",FBLUE,sslCtx->sslSock,NONE,buf);

				if (response_handler!=NULL)
				{
					((this)->response_handler)(buf);
				}
			}
			return true;
		}
		int SSLServer::SendResponseCallBack(SSLServCTX *sslCtx)
		{
			if (!sslCtx->send.send_buf)
				GenerateResponse(sslCtx);

			apr_off_t offset					= sslCtx->send.offset;
			const char *send_pt					= &sslCtx->send.send_buf[offset];
			apr_size_t len						= strlen(send_pt);
			apr_size_t wlen						= len;

			//TODO add SSL error checking and handling when connection gets dropped during file upload by th3matr1x aka Mehdi
			printf("%s\n",send_pt);
			int ssl_err = SSL_write(sslCtx->sslSock, send_pt, (int) wlen);
			if (ssl_err<1)
			{
				int error = SSL_get_error(sslCtx->sslSock, ssl_err);
				if (error==SSL_ERROR_WANT_READ)
				{
					printf("%sSSL write error , server might not be able to respond anymore%s\n",FRED,NONE);
				}
			}

			if (wlen == len)
			{

				apr_pollfd_t pfd		= { memPool, APR_POLL_SOCKET, APR_POLLOUT, 0, { NULL }, sslCtx };
				pfd.desc.s				= sslCtx->sock;
				apr_pollset_remove(pollSet, &pfd);

				pfd.reqevents 		= APR_POLLIN;
				apr_pollset_add(pollSet, &pfd);
				socketStatus 		= SERV_RECV_REQUEST;
				socket_callback_t	= &SSLServer::ReceiveRequestCallBack;
			}
			else
				sslCtx->send.offset += wlen;

			return true;
		}
		void SSLServer::GenerateResponse(SSLServCTX *sslCtx)
		{
			apr_file_t *fp;
			const char *filepath = sslCtx->recv.filePath;

			printf("path =%s %s %s",FYELLOW,filepath,NONE);

			if ((retStatus = apr_file_open(&fp, filepath, APR_READ, APR_OS_DEFAULT, memPool)) == APR_SUCCESS)
			{
				const char *resp_hdr;
				apr_size_t len;
				const char *resp_body;

				apr_finfo_t finfo;
				apr_file_info_get(&finfo, APR_FINFO_SIZE, fp);

				resp_hdr = apr_psprintf(memPool, "HTTP/1.0 200 OK" CRLF_STR "Content-Length: %" APR_OFF_T_FMT CRLF_STR CRLF_STR, finfo.size);
				resp_body = (const char*)apr_pcalloc(memPool, finfo.size + 1);
				len = (apr_size_t) finfo.size;
				apr_file_read(fp, (void*)resp_body, &len);

				// TODO for big files, needs to split the sendfile options and to send each parts
				sslCtx->send.send_buf = apr_pstrcat(memPool, resp_hdr, resp_body, NULL);
			}
			else
			{
				char *buf= apr_psprintf(memPool, "%s", FPURPLE "HTTP/1.0 404 Not Found" CRLF_STR CRLF_STR NONE);
				sslCtx->send.send_buf = buf;
			}
		}

		int SSLServer::InitializeSSL()
		{
			SSL_load_error_strings();
			SSL_library_init();
			OpenSSL_add_all_algorithms();
			return 0;
		}

		int SSLServer::ShutdownSSL(SSL* _cSSL)
		{
			SSL_shutdown(_cSSL);
			SSL_free(_cSSL);
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
			printf("%sSSL error code = %d%s",FRED,i,NONE);
			char* SSLeerordumper =ERR_error_string(ERR_get_error(), NULL);
			printf("%sSSL error dumper = %s%s",FRED,SSLeerordumper,NONE);
		}
	}
}
