#include "multiplex/NoSSLServer.h"

#define DEF_POLL_TIMEOUT	(APR_USEC_PER_SEC * 30)

namespace RLMSocketServer
{
	namespace MultiPlex
	{
		NoSSLServer::NoSSLServer(int port) : MultiPlex::BaseServer(port,RLMServerMode::NOSSLMODE)
		{
		}

		NoSSLServer::~NoSSLServer()
		{
		}

		int	NoSSLServer::RunSocketServer()
		{
			if (serverState==RLMSocketServer::INVALID)
				return 0;
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
							servCTX *sslCtx = (servCTX *)ret_pfd[i].client_data;
							((this)->*socket_callback_t)(sslCtx);
						}
					}
				}
			}
			return 0;
		}

		int	NoSSLServer::PollAccept()
		{
			apr_socket_t *ns;

			retStatus = apr_socket_accept(&ns, listenSocket, memPool);

			if (retStatus == APR_SUCCESS)
			{
				servCTX* ctx = (servCTX*)apr_palloc(memPool, sizeof(servCTX));
				ctx->sock=ns;
				printf("new client connected %s%p%s\n",FYELLOW,ns,NONE);
				apr_pollfd_t pfd = { memPool, APR_POLL_SOCKET, APR_POLLIN, 0, { NULL }, ctx };
				pfd.desc.s = ns;
				apr_socket_opt_set(ns, APR_SO_NONBLOCK, 1);
				apr_socket_timeout_set(ns, 0);
				socketStatus 		= SocketStatus::SERV_RECV_REQUEST;
				socket_callback_t	= &NoSSLServer::ReceiveRequestCallBack;
				apr_pollset_add(pollSet, &pfd);
			}
			return true;
		}

		int	NoSSLServer::ReceiveRequestCallBack(servCTX *ctx)
		{
			while (1)
			{
				char buf[BUFSIZE];
				apr_size_t len = sizeof(buf) - 1;
				retStatus = apr_socket_recv(ctx->sock, buf, &len);
				if (retStatus == APR_EOF || len == 0)
					break;

				buf[len] = '\0';

				if (strstr(buf, "GET"))
				{
					char **tokens;
					apr_tokenize_to_argv(buf, &tokens, memPool);
					if (tokens[0] && tokens[1] && strcmp(tokens[0], "GET") == 0)
					{
						ctx->recv.filePath = apr_pstrdup(memPool, tokens[1]);
						printf("requesting file %s\n",tokens[1]);
					}
				}
				if (strstr(buf, CRLF_STR CRLF_STR) && strstr(buf, "GET"))
				{
					socketStatus		= SocketStatus::SERV_SEND_RESPONSE;
					socket_callback_t 	= &NoSSLServer::SendResponseCallBack;
					apr_pollfd_t pfd 	= { memPool, APR_POLL_SOCKET, APR_POLLIN, 0, { NULL }, ctx };
					pfd.desc.s 			= ctx->sock;
					apr_pollset_remove(pollSet, &pfd);
					pfd.reqevents 		= APR_POLLOUT;
					apr_pollset_add(pollSet, &pfd);
					ctx->send.send_buf		= NULL;

					break;
				}
				buf[strcspn(buf, "\r\n")] = '\0';
				printf("%s%p:%s%s\n",FBLUE,ctx->sock,NONE,buf);
			}
			return true;
		}
		int NoSSLServer::SendResponseCallBack(servCTX *ctx)
		{
			if (!ctx->send.send_buf)
				GenerateResponse(ctx);

			apr_off_t offset			= ctx->send.offset;
			const char *send_pt			= &ctx->send.send_buf[offset];
			apr_size_t len				= strlen(send_pt);
			apr_size_t wlen				= len;

			//TODO add error checking and handling when connection gets dropped during file upload by th3matr1x aka Mehdi
			retStatus = apr_socket_send(ctx->sock, send_pt, &wlen);
			if (wlen == len)
			{
				apr_pollfd_t pfd		= { memPool, APR_POLL_SOCKET, APR_POLLOUT, 0, { NULL }, ctx };
				pfd.desc.s				= ctx->sock;
				apr_pollset_remove(pollSet, &pfd);

				pfd.reqevents 		= APR_POLLIN;
				apr_pollset_add(pollSet, &pfd);
				socketStatus 		= SocketStatus::SERV_RECV_REQUEST;
				socket_callback_t	= &NoSSLServer::ReceiveRequestCallBack;
				//apr_socket_close(sock);
			}
			else
				ctx->send.offset += wlen;

			return true;
		}
		void NoSSLServer::GenerateResponse(servCTX *ctx)
		{
			apr_file_t *fp;
			const char *filepath = ctx->recv.filePath;

			if ((retStatus = apr_file_open(&fp, filepath, APR_READ, APR_OS_DEFAULT, memPool)) == APR_SUCCESS)
			{
				const char *resp_hdr;
				apr_size_t len;
				const char *resp_body;

				apr_finfo_t finfo;
				apr_file_info_get(&finfo, APR_FINFO_SIZE, fp);

				resp_hdr = apr_psprintf(memPool, "HTTP/1.0 200 OK" CRLF_STR "Content-Length: %" APR_OFF_T_FMT CRLF_STR CRLF_STR, finfo.size);
				resp_body = (const char*)apr_pcalloc(memPool, (apr_size_t)finfo.size + 1);
				len = (apr_size_t) finfo.size;
				apr_file_read(fp, (void*)resp_body, &len);

				ctx->send.send_buf = apr_pstrcat(memPool, resp_hdr, resp_body, NULL);
			}
			else
			{
				char *buf= apr_psprintf(memPool, "%s", FPURPLE "HTTP/1.0 404 Not Found" CRLF_STR CRLF_STR NONE);
				ctx->send.send_buf = buf;
			}
		}
	}
}
