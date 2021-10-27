#ifndef SRC_MULTIPLEX_SSL_SERVER_H_
#define SRC_MULTIPLEX_SSL_SERVER_H_

#include "multiplex/BaseServer.h"

namespace RLMSocketServer
{
	namespace MultiPlex
	{
		class SSLServer : public MultiPlex::BaseServer
		{
			public:
													SSLServer(char* PASS,char* certPath,char* keyPath,int port);
				virtual 							~SSLServer();

				int									RunSocketServer();
				void								SetReadHandler(int (*fp)(void*));

			private:
				int									PollAccept();
				int									ReceiveRequestCallBack(SSLServCTX *sslCtx);
				int									SendResponseCallBack(SSLServCTX *sslCtx);
				void								GenerateResponse(SSLServCTX *sslCtx);

				int									InitializeCertification();
				int 								ShutdownSSL(SSL* _cSSL);
				int									InitializeSSL();
				int									DestroySSL();
				void								CheckError(int error,SSL* cSSL);

				int									(SSLServer::*socket_callback_t)(SSLServCTX *sslCtx)	= 0;
				int									(*response_handler)(void* pointer);

				const SSL_METHOD					*SSL_method;
				SSL_CTX								*SSL_ctx;
				std::string							SSLCertificationPassword;
				char								*mCertPath;
				char								*mKeyPath;
		};
	}
}

#endif
