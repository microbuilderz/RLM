#ifndef SRC_MULTI_THREAD_NO_SSL_SERVER_H_
#define SRC_MULTI_THREAD_NO_SSL_SERVER_H_

#include "multithread/BaseServer.h"

namespace RLMSocketServer
{
	namespace MultiThread
	{
		class NoSSLServer: public MultiThread::BaseServer
		{
			public:
											NoSSLServer(apr_port_t port);
				virtual 					~NoSSLServer();

				int							RunSocketServer();
		};
	}
}

#endif
