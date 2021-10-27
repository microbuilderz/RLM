#ifndef SRC_MULTIPLEX_NO_SSL_SERVER_H_
#define SRC_MULTIPLEX_NO_SSL_SERVER_H_

#include "multiplex/BaseServer.h"

namespace RLMSocketServer
{
	namespace MultiPlex
	{
		class NoSSLServer	: public MultiPlex::BaseServer
		{
			public:
													NoSSLServer(int port);
				virtual 							~NoSSLServer();

				int									RunSocketServer();

			private:
				int									PollAccept();
				int									ReceiveRequestCallBack(servCTX *ctx);
				int									SendResponseCallBack(servCTX *ctx);
				void								GenerateResponse(servCTX *ctx);

				int	(NoSSLServer::*socket_callback_t)(servCTX *ctx)	= 0;
		};
	}
}

#endif
