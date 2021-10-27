#ifndef SRC_MULTI_THREAD_SSL_SERVER_H_
#define SRC_MULTI_THREAD_SSL_SERVER_H_

#include "multithread/BaseServer.h"

namespace RLMSocketServer
{
	namespace MultiThread
	{
		class SSLServer: public MultiThread::BaseServer
		{
			public:
										SSLServer(char* PASS,char* certPath,char* keyPath,apr_port_t port);
				virtual 				~SSLServer();

				int						RunSocketServer();

			private:
				int						InitializeCertification();
				int 					ShutdownSSL();
				int						InitializeSSL();
				int						DestroySSL();
				void					CheckError(int error,SSL* cSSL);

				const SSL_METHOD		*SSL_method;
				SSL_CTX					*SSL_ctx;
				std::string				SSLCertificationPassword;
				char					*mCertPath;
				char					*mKeyPath;
				SSL						*cSSL;
		};
	}
}

#endif
